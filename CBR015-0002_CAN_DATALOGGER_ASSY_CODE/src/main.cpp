/*
 * Copyright 2018 THOMAS NAUGHTON
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * CBR015-0002_CAN_DATALOGGER_ASSY_CODE
 * Version 1.0  09-12-2018
 * Version 1.1  03-08-2019  Added data recieved from CBR015-0004_GPS_CAN_MODULE_ASSY_CODE_v1.0
 * Version 1.2  30-12-2019  Added EGT data recieved from 015-0012_K-TYPES_MODULE_ASSY_CODE_v1.0
 * Version 2.0  25-02-2020  Adjusted data struct to suit optimised .dbc config file and Python post processing. Raw message data passed directly to struct. 
 * Version 3.0  04-06-2020  Updated code to work with PlatformIO. CAN message IDs adjusted to suit F88R datastream
 * Version 4.0  17-04-2025  Adusted for more end-user configurability.
 *                          Enhancements include, error reporting via LED, config load from file on SD card, dynamic CAN ID mapping, buffering of 
 *                          records and optimised SD card writes
 * 
 * This code is designed for use with the CBR015-0002 CAN DATALOGGER ASSY utilising CBR015-0003 Rev00 PCB layout
 * Messages recieved via CAN are MSB First or Big Endian byte order. Time signal is saved to file in MSB Last or Little Endian byte order.
 *
 * config.txt file is required on SD card. Config fields:
 *  -  filename_prefix=<text string which will be prefixed to each log file>
 *  -  log_frequency=<logging frequency in Hz>
 *  -  can_speed=<CAN Bus speed in bps>
 *  -  can_ids=<comma seperated list of messgae ID's to be logged in hex>
 * 
 * See documentation for details & overview
 */

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include <FlexCAN.h>
#include <TimeLib.h>
#include <map>
#include <vector>
#include <array>

// ======================== CONFIGURABLE LIMITS ========================
#define MAX_CAN_MESSAGES 64  // Max messages supported in one log
#define RECORD_ALIGN_BYTES 512
#define SD_CONFIG_FILE "config.txt"
#define DEFAULT_FILE_PREFIX ""
#define LED_PIN 13
#define DEFAULT_LOG_FREQ 20
#define DEFAULT_CAN_SPEED 500000

// ======================== STRUCTS ========================
struct LogHeader {
  uint32_t magic;           // 'CAND'
  uint16_t version;         // Format version
  uint16_t record_size;     // Size of a data record
  uint16_t record_count;    // Number of records per buffer write
  uint16_t log_frequency;   // Hz
  uint32_t can_speed;       // CAN baud rate
  uint8_t  message_count;   // Count of CAN messages
  uint32_t message_ids[MAX_CAN_MESSAGES];
};

// ======================== GLOBALS ========================
// SD card pins
const uint8_t chipSelect = 15;
const uint8_t mosi = 7;
const uint8_t miso = 8;
const uint8_t sck = 14;

// SD and file objects
SdFat sd;
SdFile file;

// CAN message storage
CAN_message_t rxmsg;
std::vector<uint32_t> loggedMessageIDs;
std::map<uint32_t, std::array<byte, 8>> activeCANMessages;

// Configuration variables
char filePrefix[20] = DEFAULT_FILE_PREFIX;
uint32_t canSpeed = DEFAULT_CAN_SPEED;
uint16_t logFreq = DEFAULT_LOG_FREQ;
uint16_t recordSize;
uint16_t recordsPerBlock;
uint16_t bufferSize;

// Logging state
uint8_t* logBuffer = nullptr;
uint16_t bufferIndex = 0;
bool logStarted = false;
uint32_t logStartMillis;
uint32_t prevMicros = 0;
uint32_t logPeriod;

// Status indicators
bool sdCardInitialized = false;
bool configLoaded = false;
uint32_t errorBlinkStart = 0;
uint8_t errorCode = 0;

// Error codes
enum ErrorCode {
  ERROR_NONE = 0,
  ERROR_SD_INIT = 1,
  ERROR_CONFIG_FILE = 2, 
  ERROR_FILE_OPEN = 3,
  ERROR_MEMORY = 4
};

// ======================== UTILITY ========================
time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

void indicateError(ErrorCode code) {
  errorCode = code;
  errorBlinkStart = millis();
  // Error will be indicated by LED blinking pattern in main loop
}

void blinkError() {
  // Serial.println(errorCode);

  if (errorCode == ERROR_NONE) return;
  
  uint32_t now = millis();
  uint32_t cycle = (now - errorBlinkStart) % 2000;
  
  if (cycle < 1000) {
    // On-Off pattern based on error code
    if ((cycle % 500) < (errorCode * 100)) {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

bool loadConfig() {
  File config = sd.open(SD_CONFIG_FILE);
  if (!config) {
    indicateError(ERROR_CONFIG_FILE);
    return false;
  }

  while (config.available()) {
    String line = config.readStringUntil('\n');
    line.trim();

    if (line.startsWith("filename_prefix=")) {
      line.remove(0, 16);
      line.toCharArray(filePrefix, sizeof(filePrefix));
    } else if (line.startsWith("log_frequency=")) {
      logFreq = constrain(line.substring(14).toInt(), 1, 100); // Reasonable limits
    } else if (line.startsWith("can_speed=")) {
      uint32_t speed = line.substring(10).toInt();
      // Validate against common CAN speeds
      if (speed == 125000 || speed == 250000 || speed == 500000 || speed == 800000 || speed == 1000000) {
        canSpeed = speed;
      }
    } else if (line.startsWith("can_ids=")) {
      String ids = line.substring(8);
      while (ids.length() && loggedMessageIDs.size() < MAX_CAN_MESSAGES) {
        int comma = ids.indexOf(',');
        String idStr = (comma == -1) ? ids : ids.substring(0, comma);
        idStr.trim();
        if (idStr.length() > 0) {
          uint32_t id = strtol(idStr.c_str(), nullptr, 16);
          if (id > 0) {  // Validate ID
            loggedMessageIDs.push_back(id);
            activeCANMessages[id] = {0};
          }
        }
        if (comma == -1) break;
        ids = ids.substring(comma + 1);
      }
    }
  }
  config.close();
  
  // Ensure we have at least one message ID
  if (loggedMessageIDs.empty()) {
    return false;
  }
  
  return true;
}

void writeHeader() {
  LogHeader header;
  header.magic = 0x43414E44;  // 'CAND'
  header.version = 1;
  header.record_size = recordSize;
  header.record_count = recordsPerBlock;
  header.log_frequency = logFreq;
  header.can_speed = canSpeed;
  header.message_count = loggedMessageIDs.size();
  
  for (size_t i = 0; i < header.message_count; i++) {
    header.message_ids[i] = loggedMessageIDs[i];
  }
  
  file.write((const uint8_t*)&header, sizeof(LogHeader));
  file.sync();
}

void canReceive() {
  while (Can0.available()) {
    Can0.read(rxmsg);
    auto it = activeCANMessages.find(rxmsg.id);
    if (it != activeCANMessages.end()) {
      std::copy(rxmsg.buf, rxmsg.buf + 8, it->second.begin());
    }
  }
}

void logData() {
  if (!logStarted) {
    logStartMillis = millis();
    logStarted = true;
  }

  uint32_t timestamp = millis() - logStartMillis;
  uint16_t offset = 0;
  
  // Write timestamp
  memcpy(logBuffer + bufferIndex * recordSize, &timestamp, sizeof(uint32_t));
  offset += sizeof(uint32_t);

  // Write CAN data
  for (uint32_t id : loggedMessageIDs) {
    const auto& msg = activeCANMessages[id];
    memcpy(logBuffer + bufferIndex * recordSize + offset, msg.data(), 8);
    offset += 8;
  }

  bufferIndex++;
  
  // If buffer is full, write to SD
  if (bufferIndex >= recordsPerBlock) {
    file.write(logBuffer, bufferSize);
    file.sync();
    bufferIndex = 0;
  }
}

void calculateBufferSize() {
  // Calculate record size based on message count (round up to power of 2)
  uint16_t baseSize = sizeof(uint32_t) + loggedMessageIDs.size() * 8;
  
  if (baseSize <= 32) recordSize = 32;
  else if (baseSize <= 64) recordSize = 64;
  else if (baseSize <= 128) recordSize = 128;
  else if (baseSize <= 256) recordSize = 256;
  else recordSize = 512;
  
  recordsPerBlock = RECORD_ALIGN_BYTES / recordSize;
  bufferSize = recordsPerBlock * recordSize;
}

// ======================== SETUP ========================
void setup() {

  // Serial.begin(115200);
  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(chipSelect, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // Turn on LED during initialization
  
  // Configure SPI for SD card
  SPI.setMOSI(mosi);
  SPI.setMISO(miso);
  SPI.setSCK(sck);
  delay(100);  // Short delay to ensure SD card is powered up
  
  // Set up time provider
  setSyncProvider(getTeensy3Time);
  
  // Initialize SD card
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    indicateError(ERROR_SD_INIT);
    return;
  }
  sdCardInitialized = true;
  
  // Load configuration
  if (!loadConfig()) {
    // If config load fails, fall back to defaults
    if (loggedMessageIDs.empty()) {
      // Add a default ID to monitor if none specified
      loggedMessageIDs.push_back(0x7DF);  // OBD-II broadcast ID
      activeCANMessages[0x7DF] = {0};
    }
  }
  configLoaded = true;
  
  // Calculate logging parameters
  logPeriod = 1000000UL / logFreq;
  calculateBufferSize();
  
  // Allocate buffer memory
  logBuffer = (uint8_t*)malloc(bufferSize);
  if (!logBuffer) {
    indicateError(ERROR_MEMORY);
    return;
  }
  memset(logBuffer, 0, bufferSize);
  
  // Initialize CAN bus
  Can0.begin(canSpeed);
  
  // Create log file with timestamp in name
  // char timestamp[15];
  // sprintf(timestamp, "%04d%02d%02d%02d%02d%02d", 
  //         year(), month(), day(), hour(), minute(), second());
  char filename[32];
  sprintf(filename, "%s%04d%02d%02d_%02d%02d%02d.dat", 
          filePrefix, year(), month(), day(), hour(), minute(), second());
          
  if (!file.open(filename, O_CREAT | O_WRITE | O_EXCL)) {
    indicateError(ERROR_FILE_OPEN);
    return;
  }
  
  // Write file header
  writeHeader();
  
  // Initialization complete
  digitalWrite(LED_PIN, LOW);
}

// ======================== LOOP ========================
void loop() {
  // Handle errors
  if (errorCode != ERROR_NONE) {
    blinkError();
    return;  // Don't continue if there are errors
  }
  
  // Process incoming CAN messages
  canReceive();

  // Check if it's time to log data
  uint32_t now = micros();
  if ((now - prevMicros) >= logPeriod) {
    prevMicros = now;
    logData();
    
    // Toggle LED to indicate logging activity
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
}
