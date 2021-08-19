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
 * 
 * This code is designed for use with the CBR015-0002 CAN DATALOGGER ASSY utilising CBR015-0003 Rev00 PCB layout
 * CAN Config: 200225_LOGGER_CAN_CONFIG_v1.3.xlsx
 * Data File Converter: CBR015-0002_Bin_File_Converter_v2.1.0.m
 * Converter Input Config: CBR015-0002_CAN_LOGGER_CONFIG_v1.0.dbc
 * Messages recieved via CAN are MSB First or Big Endian byte order. Time signal is saved to file in MSB Last or Little Endian byte order.
 * 
 * See documentation for details & overview
 */

#include <Arduino.h>

#include <SPI.h>
#include <SdFat.h>
#include <TimeLib.h>
//#include <string.h>
#include <FlexCAN.h>

// ===============================================================================================================
// Declare variables most likely to be adjusted by user in this block

#define log_freq 20      // Logging rate in Hz
#define can_speed 500000 // CAN-Bus baud rate in bps

// ===============================================================================================================
// Declare Data Variable Name Arrays here

// Parameter names represent message ID. Precision is always a byte array with size 8 (full 8 byte CAN message)
struct ECU_Data
{
  uint64_t ID_5FF; // Time is different from rest as it is not recieved as CAN message
  byte ID_600[8];
  byte ID_601[8];
  byte ID_602[8];
  byte ID_603[8];
  byte ID_604[8];
  byte ID_605[8];
  byte ID_606[8];
  byte ID_607[8];
  byte ID_608[8];
  byte ID_609[8];
  byte ID_60A[8];
  byte ID_60B[8];
  byte ID_60C[8];
  byte ID_60D[8];
  byte ID_60E[8];
  byte ID_60F[8];
  byte ID_610[8];
  byte ID_611[8];
  byte ID_612[8];
  byte ID_640[8];
  byte ID_641[8];
  byte ID_642[8];
  byte ID_680[8];
  byte ID_681[8];
  byte ID_682[8];
  byte ID_683[8];
  byte ID_684[8];
  byte ID_685[8];
  byte ID_174[8];
  byte ID_178[8];
  byte ID_17C[8];
  // try to maintain struct size at 256 bytes, allowing potential to use 2 record buffer to increase data write speeds.
};

struct ECU_Data Data; // This struct will only ever have one instance at a time & needs to be accessed by all functions

// ===============================================================================================================

// Initialise SPI pin numbers according to CBR015-0003 Rev00 schematic
#define chipSelect 15
#define mosi 7
#define miso 8
#define sck 14

// Initialise can bus and can messages
static CAN_message_t rxmsg;

// Declare variables used by logger
uint16_t log_period, diff;
uint32_t LogStartMillis, prev_time, now_time, CANReadTime;
bool LogStarted;
bool dataFile = 0;

//------------------------------------------------------------------------------
// File system object.
SdFat sd;

// Log file.
SdFile file;

// ===============================================================================================================
// User functions.  Edit writeHeader() and logData() for your requirements.

// Error messages stored in flash.
#define error(msg) sd.errorHalt(F(msg))

// ===============================================================================================================
// Log a data record.
void logData()
{

  if (LogStarted == false)
  {                            //check log started flag
    LogStartMillis = millis(); // store time from first log. This is used to calculate Time variable (time since log start in seconds)
    LogStarted = true;         //set log started flag to true to avoid this section of code from running again
  }

  uint64_t Time = (millis() - LogStartMillis); // calculate time since log start in ms

  Data.ID_5FF = Time;

  file.write((const uint8_t *)&Data, sizeof(Data));
}

// ===============================================================================================================

void CopyArray(byte src[8], byte dest[8])
{
  // function to manage copying of CAN message byte arrays to data structure while maintaining Endian-ness

  uint8_t i;

  for (i = 0; i < 8; i++)
  {
    dest[i] = src[i];
  }
}

// ===============================================================================================================

void can_recieve()
{

  if (Can0.available())
  {

    Can0.read(rxmsg);

    switch (rxmsg.id)
    {

    // with each message data, we copy the bytes individually across to the data struct array to maintain Endian-ness
    case 0x600:
      CopyArray(rxmsg.buf, Data.ID_600);
      break;
    case 0x601:
      CopyArray(rxmsg.buf, Data.ID_601);
      break;
    case 0x602:
      CopyArray(rxmsg.buf, Data.ID_602);
      break;
    case 0x603:
      CopyArray(rxmsg.buf, Data.ID_603);
      break;
    case 0x604:
      CopyArray(rxmsg.buf, Data.ID_604);
      break;
    case 0x605:
      CopyArray(rxmsg.buf, Data.ID_605);
      break;
    case 0x606:
      CopyArray(rxmsg.buf, Data.ID_606);
      break;
    case 0x607:
      CopyArray(rxmsg.buf, Data.ID_607);
      break;
    case 0x608:
      CopyArray(rxmsg.buf, Data.ID_608);
      break;
    case 0x609:
      CopyArray(rxmsg.buf, Data.ID_609);
      break;
    case 0x60A:
      CopyArray(rxmsg.buf, Data.ID_60A);
      break;
    case 0x60B:
      CopyArray(rxmsg.buf, Data.ID_60B);
      break;
    case 0x60C:
      CopyArray(rxmsg.buf, Data.ID_60C);
      break;
    case 0x60D:
      CopyArray(rxmsg.buf, Data.ID_60D);
      break;
    case 0x60E:
      CopyArray(rxmsg.buf, Data.ID_60E);
      break;
    case 0x60F:
      CopyArray(rxmsg.buf, Data.ID_60F);
      break;
    case 0x610:
      CopyArray(rxmsg.buf, Data.ID_610);
      break;
    case 0x611:
      CopyArray(rxmsg.buf, Data.ID_611);
      break;
    case 0x612:
      CopyArray(rxmsg.buf, Data.ID_612);
      break;
    case 0x640:
      CopyArray(rxmsg.buf, Data.ID_640);
      break;
    case 0x641:
      CopyArray(rxmsg.buf, Data.ID_641);
      break;
    case 0x642:
      CopyArray(rxmsg.buf, Data.ID_642);
      break;
    case 0x680:
      CopyArray(rxmsg.buf, Data.ID_680);
      break;
    case 0x681:
      CopyArray(rxmsg.buf, Data.ID_681);
      break;
    case 0x682:
      CopyArray(rxmsg.buf, Data.ID_682);
      break;
    case 0x683:
      CopyArray(rxmsg.buf, Data.ID_683);
      break;
    case 0x684:
      CopyArray(rxmsg.buf, Data.ID_684);
      break;
    case 0x685:
      CopyArray(rxmsg.buf, Data.ID_685);
      break;
    case 0x174:
      CopyArray(rxmsg.buf, Data.ID_174);
      break;
    case 0x178:
      CopyArray(rxmsg.buf, Data.ID_178);
      break;
    case 0x17C:
      CopyArray(rxmsg.buf, Data.ID_17C);
      break;
    }
  }
}

// ===============================================================================================================

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

// ===============================================================================================================
void setup()
{

  //Serial.begin(9600); //debug only

  // Set up SD card pins. SdFat cannot access card without these declarations
  SPI.setMOSI(mosi);
  SPI.setMISO(miso);
  SPI.setSCK(sck);
  pinMode(chipSelect, OUTPUT);
  pinMode(13, OUTPUT);

  delay(1000);

  setSyncProvider(getTeensy3Time); // use teensy rtc as time source

  // Create data file name using timestamp
  char timestamp[15];
  sprintf(timestamp, "%04d%02d%02d%02d%02d%02d", year(), month(), day(), hour(), minute(), second());
  char fileName[30]; //= "CBR250RRi_" timestamp;
  strcpy(fileName, "CBR250RRi_");
  strcat(fileName, timestamp);
  strcat(fileName, ".dat");

  log_period = (1000000 / log_freq); // calculate log period in µs from frequency

  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50)))
  {
    sd.initErrorHalt();
  }

  if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL))
  {
    error("file.open");
  }

  //initialise log started flag
  LogStarted = false;

  file.open(fileName, O_WRITE);

  //Serial.print(F("Logging to: ")); // debug only
  //Serial.println(fileName);

  // Initialise CAN
  Can0.begin(can_speed);

  digitalWrite(13, HIGH); //Turn on on-board LED for power indication only
}

// ===============================================================================================================
void loop()
{

  can_recieve(); // Carry out CAN recieve function as often as possible to reduce missed data

  // Time for next record.
  now_time = micros();
  diff = (now_time - prev_time); // need to check this part to ensure it can deal with micros() overflow

  if (diff >= log_period)
  {
    prev_time = now_time;

    logData();

    // Force data to SD and update the directory entry to avoid data loss.
    if (!file.sync() || file.getWriteError())
    {
      error("write error"); //debug only
    }
  }
}