EESchema Schematic File Version 4
LIBS:CBR015-0003_CAN_DATALOGGER_PCB-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "CAN DATALOGGER PCB"
Date "22-02-18"
Rev "00"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "CBR015-0003"
$EndDescr
$Comp
L CBR015-0003_CAN_DATALOGGER_PCB-rescue:MCP2551-I_SN U3
U 1 1 5A8F38E0
P 2200 6200
F 0 "U3" H 1800 6550 50  0000 L CNN
F 1 "MCP2551-I/SN" H 2300 6550 50  0000 L CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 2200 5700 50  0001 C CIN
F 3 "" H 2200 6200 50  0000 C CNN
	1    2200 6200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5A8F3957
P 1950 5600
F 0 "C2" V 2000 5400 50  0000 L CNN
F 1 "100nF" V 2100 5400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1988 5450 50  0001 C CNN
F 3 "" H 1950 5600 50  0000 C CNN
	1    1950 5600
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 5A8F3A22
P 1450 6400
F 0 "R1" V 1530 6400 50  0000 C CNN
F 1 "18K" V 1450 6400 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 1380 6400 50  0001 C CNN
F 3 "" H 1450 6400 50  0000 C CNN
	1    1450 6400
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR01
U 1 1 5A8F3B3C
P 2200 5550
F 0 "#PWR01" H 2200 5400 50  0001 C CNN
F 1 "+5V" H 2200 5690 50  0000 C CNN
F 2 "" H 2200 5550 50  0000 C CNN
F 3 "" H 2200 5550 50  0000 C CNN
	1    2200 5550
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR02
U 1 1 5A8F3B5C
P 2200 6650
F 0 "#PWR02" H 2200 6400 50  0001 C CNN
F 1 "Earth" H 2200 6500 50  0001 C CNN
F 2 "" H 2200 6650 50  0000 C CNN
F 3 "" H 2200 6650 50  0000 C CNN
	1    2200 6650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5A8F3B80
P 3600 6300
F 0 "R2" V 3680 6300 50  0000 C CNN
F 1 "120" V 3600 6300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3530 6300 50  0001 C CNN
F 3 "" H 3600 6300 50  0000 C CNN
	1    3600 6300
	-1   0    0    1   
$EndComp
Wire Wire Line
	2200 6600 2200 6650
Wire Wire Line
	1600 6400 1700 6400
$Comp
L power:Earth #PWR03
U 1 1 5A8F3C40
P 1200 6400
F 0 "#PWR03" H 1200 6150 50  0001 C CNN
F 1 "Earth" H 1200 6250 50  0001 C CNN
F 2 "" H 1200 6400 50  0000 C CNN
F 3 "" H 1200 6400 50  0000 C CNN
	1    1200 6400
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR04
U 1 1 5A8F3C6B
P 1600 5600
F 0 "#PWR04" H 1600 5350 50  0001 C CNN
F 1 "Earth" H 1600 5450 50  0001 C CNN
F 2 "" H 1600 5600 50  0000 C CNN
F 3 "" H 1600 5600 50  0000 C CNN
	1    1600 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 5600 1800 5600
Wire Wire Line
	2200 5550 2200 5600
Wire Wire Line
	2100 5600 2200 5600
Connection ~ 2200 5600
Text GLabel 1550 5950 0    60   Input ~ 0
TX_CAN
Text GLabel 1550 6150 0    60   Input ~ 0
RX_CAN
Wire Wire Line
	1550 6150 1650 6150
Wire Wire Line
	1650 6150 1650 6100
Wire Wire Line
	1650 6100 1700 6100
Wire Wire Line
	1550 5950 1650 5950
Wire Wire Line
	1650 5950 1650 6000
Wire Wire Line
	1650 6000 1700 6000
Text GLabel 3000 5800 1    60   Input ~ 0
CANH
Text GLabel 3750 6450 2    60   Input ~ 0
CANL
Wire Wire Line
	2700 6100 3000 6100
Wire Wire Line
	3000 5800 3000 6100
Connection ~ 3000 6100
Wire Wire Line
	3600 6050 3600 6150
Wire Wire Line
	3150 6450 3600 6450
Wire Wire Line
	3150 6450 3150 6300
Wire Wire Line
	3150 6300 2700 6300
Connection ~ 3600 6450
Text Notes 1850 6900 0    60   ~ 0
CAN TRANSCEIVER
$Comp
L power:+12V #PWR05
U 1 1 5A8F4A6E
P 5400 800
F 0 "#PWR05" H 5400 650 50  0001 C CNN
F 1 "+12V" H 5400 940 50  0000 C CNN
F 2 "" H 5400 800 50  0000 C CNN
F 3 "" H 5400 800 50  0000 C CNN
	1    5400 800 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5A8F4B78
P 5500 1050
F 0 "C1" H 5525 1150 50  0000 L CNN
F 1 "22µF" H 5525 950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5538 900 50  0001 C CNN
F 3 "" H 5500 1050 50  0000 C CNN
	1    5500 1050
	-1   0    0    1   
$EndComp
$Comp
L power:Earth #PWR06
U 1 1 5A8F4EEE
P 5950 1350
F 0 "#PWR06" H 5950 1100 50  0001 C CNN
F 1 "Earth" H 5950 1200 50  0001 C CNN
F 2 "" H 5950 1350 50  0000 C CNN
F 3 "" H 5950 1350 50  0000 C CNN
	1    5950 1350
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR07
U 1 1 5A8F51FF
P 6500 800
F 0 "#PWR07" H 6500 650 50  0001 C CNN
F 1 "+5V" H 6500 940 50  0000 C CNN
F 2 "" H 6500 800 50  0000 C CNN
F 3 "" H 6500 800 50  0000 C CNN
	1    6500 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 850  6500 850 
Wire Wire Line
	6500 850  6500 800 
Wire Wire Line
	5400 850  5500 850 
Wire Wire Line
	5500 900  5500 850 
Connection ~ 5500 850 
Wire Wire Line
	5950 1250 5950 1300
Wire Wire Line
	5500 1200 5500 1300
Wire Wire Line
	5500 1300 5950 1300
Connection ~ 5950 1300
Text Notes 5250 1650 0    60   ~ 0
BOARD POWER VOLTAGE REGULATOR
$Comp
L User_Library:TSR_1-2450 U2
U 1 1 5A8F6488
P 5950 900
F 0 "U2" H 5800 1200 60  0000 C CNN
F 1 "TSR_1-2450" H 6000 1100 60  0000 C CNN
F 2 "Converters_DCDC_ACDC:DCDC-Conv_TRACO_TSR-1" H 5950 900 60  0001 C CNN
F 3 "" H 5950 900 60  0001 C CNN
	1    5950 900 
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG08
U 1 1 5A8F6AD8
P 7000 1150
F 0 "#FLG08" H 7000 1245 50  0001 C CNN
F 1 "PWR_FLAG" H 7000 1330 50  0000 C CNN
F 2 "" H 7000 1150 50  0000 C CNN
F 3 "" H 7000 1150 50  0000 C CNN
	1    7000 1150
	-1   0    0    1   
$EndComp
$Comp
L power:+12V #PWR09
U 1 1 5A8F6E60
P 7000 1150
F 0 "#PWR09" H 7000 1000 50  0001 C CNN
F 1 "+12V" H 7000 1290 50  0000 C CNN
F 2 "" H 7000 1150 50  0000 C CNN
F 3 "" H 7000 1150 50  0000 C CNN
	1    7000 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 800  5400 850 
NoConn ~ 1700 6300
Text GLabel 3100 6100 2    60   Input ~ 0
TERMH
Text GLabel 3600 6050 1    60   Input ~ 0
TERML
Text Notes 5200 6250 0    60   ~ 0
3V CELL BATTERY
$Comp
L power:Earth #PWR010
U 1 1 5B42B281
P 8150 3650
F 0 "#PWR010" H 8150 3400 50  0001 C CNN
F 1 "Earth" H 8150 3500 50  0001 C CNN
F 2 "" H 8150 3650 50  0000 C CNN
F 3 "" H 8150 3650 50  0000 C CNN
	1    8150 3650
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR011
U 1 1 5B42B662
P 8150 3400
F 0 "#PWR011" H 8150 3250 50  0001 C CNN
F 1 "+3.3V" H 8150 3540 50  0000 C CNN
F 2 "" H 8150 3400 50  0000 C CNN
F 3 "" H 8150 3400 50  0000 C CNN
	1    8150 3400
	1    0    0    -1  
$EndComp
Text GLabel 7450 3350 2    60   Input ~ 0
SD_CS
Text GLabel 3900 3450 0    60   Input ~ 0
SD_MOSI
Text GLabel 3900 3600 0    60   Input ~ 0
SD_SCK
Text GLabel 3900 3750 0    60   Input ~ 0
SD_MISO
Text GLabel 7450 3800 2    60   Input ~ 0
SD_DETECT
Text GLabel 1150 1550 0    60   Input ~ 0
SD_DETECT
Text GLabel 1150 1850 0    60   Input ~ 0
SD_MISO
Text GLabel 2800 950  2    60   Input ~ 0
SD_SCK
Text GLabel 1150 1700 0    60   Input ~ 0
SD_MOSI
Text GLabel 2800 1100 2    60   Input ~ 0
SD_CS
Wire Wire Line
	2500 1000 2650 1000
Wire Wire Line
	2650 1000 2650 950 
Wire Wire Line
	2650 950  2800 950 
Wire Wire Line
	2500 1100 2800 1100
$Comp
L power:+5V #PWR012
U 1 1 5B43C298
P 2800 2200
F 0 "#PWR012" H 2800 2050 50  0001 C CNN
F 1 "+5V" H 2800 2340 50  0000 C CNN
F 2 "" H 2800 2200 50  0000 C CNN
F 3 "" H 2800 2200 50  0000 C CNN
	1    2800 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 2200 2800 2200
Text GLabel 1150 1250 0    60   Input ~ 0
TX_CAN
Text GLabel 1150 1400 0    60   Input ~ 0
RX_CAN
$Comp
L power:+3.3V #PWR013
U 1 1 5B43D05E
P 2800 2000
F 0 "#PWR013" H 2800 1850 50  0001 C CNN
F 1 "+3.3V" H 2800 2140 50  0000 C CNN
F 2 "" H 2800 2000 50  0000 C CNN
F 3 "" H 2800 2000 50  0000 C CNN
	1    2800 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 2000 2800 2000
Text GLabel 8700 2250 0    60   Input ~ 0
TERMH
Text GLabel 9600 2250 2    60   Input ~ 0
TERML
Text Notes 8550 2550 0    60   ~ 0
CAN TERMINATION JUMPER
$Comp
L conn:CONN_01X04 P1
U 1 1 5B4E6405
P 9400 5250
F 0 "P1" H 9400 5500 50  0000 C CNN
F 1 "CONN_01X04" V 9500 5250 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_2x02_Pitch2.54mm" H 9400 5250 50  0001 C CNN
F 3 "" H 9400 5250 50  0000 C CNN
	1    9400 5250
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR014
U 1 1 5B4E65E9
P 9000 5000
F 0 "#PWR014" H 9000 4850 50  0001 C CNN
F 1 "+12V" H 9000 5140 50  0000 C CNN
F 2 "" H 9000 5000 50  0000 C CNN
F 3 "" H 9000 5000 50  0000 C CNN
	1    9000 5000
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR015
U 1 1 5B4E68F6
P 8750 5050
F 0 "#PWR015" H 8750 4800 50  0001 C CNN
F 1 "Earth" H 8750 4900 50  0001 C CNN
F 2 "" H 8750 5050 50  0000 C CNN
F 3 "" H 8750 5050 50  0000 C CNN
	1    8750 5050
	1    0    0    -1  
$EndComp
Text GLabel 8900 5450 0    60   Input ~ 0
CANH
Text GLabel 8900 5300 0    60   Input ~ 0
CANL
Wire Wire Line
	9200 5100 9000 5100
Wire Wire Line
	9000 5100 9000 5000
Wire Wire Line
	9200 5200 8900 5200
Wire Wire Line
	8900 5200 8900 5050
Wire Wire Line
	8900 5050 8750 5050
Wire Wire Line
	8900 5300 9200 5300
Wire Wire Line
	9200 5400 9000 5400
Wire Wire Line
	9000 5400 9000 5450
Text Notes 5350 4050 0    60   ~ 0
MICRO SD CARD
Text Notes 8700 5800 0    60   ~ 0
HARNESS CONNECTOR
Text GLabel 1750 2900 3    60   Input ~ 0
VBAT
Wire Wire Line
	1750 2700 1750 2900
Text Notes 1400 3500 0    60   ~ 0
TEENSY 3.2 CONTROLLER
$Comp
L power:Earth #PWR016
U 1 1 5B4EA971
P 1100 900
F 0 "#PWR016" H 1100 650 50  0001 C CNN
F 1 "Earth" H 1100 750 50  0001 C CNN
F 2 "" H 1100 900 50  0000 C CNN
F 3 "" H 1100 900 50  0000 C CNN
	1    1100 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 900  1400 900 
Wire Wire Line
	1250 1300 1250 1250
Wire Wire Line
	1250 1250 1150 1250
Wire Wire Line
	1250 1550 1150 1550
Wire Wire Line
	1150 1850 1200 1850
Wire Wire Line
	1200 1850 1200 1800
Wire Wire Line
	1200 1800 1400 1800
Wire Wire Line
	1150 1700 1400 1700
Wire Wire Line
	1250 1550 1250 1600
Wire Wire Line
	1250 1600 1400 1600
NoConn ~ 1400 1900
NoConn ~ 1400 2000
NoConn ~ 1400 2100
NoConn ~ 1400 2200
NoConn ~ 1400 1500
NoConn ~ 1400 1200
NoConn ~ 1400 1100
NoConn ~ 1400 1000
NoConn ~ 2500 900 
NoConn ~ 2500 1200
NoConn ~ 2500 1300
NoConn ~ 2500 1400
NoConn ~ 2500 1500
NoConn ~ 2500 1600
NoConn ~ 2500 1700
NoConn ~ 2500 1800
NoConn ~ 2500 1900
NoConn ~ 2500 2100
Wire Wire Line
	1250 1300 1400 1300
Wire Wire Line
	1150 1400 1400 1400
$Comp
L SamacSys_Parts:IKD0103101 S1
U 1 1 5B4F12C1
P 8750 2250
F 0 "S1" H 9400 2550 50  0000 L CNN
F 1 "IKD0103101" H 9400 2450 50  0000 L CNN
F 2 "SamacSys_Parts:IKD0103101" H 9400 2350 50  0001 L CNN
F 3 "http://uk.rs-online.com/web/p/products/8772242" H 9400 2250 50  0001 L CNN
F 4 "Switch Dip 1 pos SMT Raised Act slide 1 Way Surface Mount DIP Switch SPST, Raised Actuator Slide Actuator" H 9400 2150 50  0001 L CNN "Description"
F 5 "" H 9400 2050 50  0001 L CNN "Height"
F 6 "8772242" H 9400 1950 50  0001 L CNN "RS Part Number"
F 7 "Apem" H 9400 1850 50  0001 L CNN "Manufacturer_Name"
F 8 "IKD0103101" H 9400 1750 50  0001 L CNN "Manufacturer_Part_Number"
	1    8750 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 2250 8750 2250
Wire Wire Line
	9550 2250 9600 2250
$Comp
L SamacSys_Parts:3001 U4
U 1 1 5B4F2054
P 5050 5800
F 0 "U4" H 6000 6100 50  0000 L CNN
F 1 "3001" H 6000 6000 50  0000 L CNN
F 2 "SamacSys_Parts:3001" H 6000 5900 50  0001 L CNN
F 3 "http://uk.rs-online.com/web/p/products/2508837311" H 6000 5800 50  0001 L CNN
F 4 "Coin Cell Battery Holders TH COIN CELL BATTERY HOLDER 12mm" H 6000 5700 50  0001 L CNN "Description"
F 5 "" H 6000 5600 50  0001 L CNN "Height"
F 6 "2508837311" H 6000 5500 50  0001 L CNN "RS Part Number"
F 7 "Keystone Electronics" H 6000 5400 50  0001 L CNN "Manufacturer_Name"
F 8 "3001" H 6000 5300 50  0001 L CNN "Manufacturer_Part_Number"
F 9 "70182368" H 6000 5200 50  0001 L CNN "Allied_Number"
	1    5050 5800
	1    0    0    -1  
$EndComp
Text GLabel 4850 5800 0    60   Input ~ 0
VBAT
$Comp
L power:Earth #PWR017
U 1 1 5B4F2491
P 6250 5850
F 0 "#PWR017" H 6250 5600 50  0001 C CNN
F 1 "Earth" H 6250 5700 50  0001 C CNN
F 2 "" H 6250 5850 50  0000 C CNN
F 3 "" H 6250 5850 50  0000 C CNN
	1    6250 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 5900 4950 5900
Wire Wire Line
	4850 5800 4950 5800
Wire Wire Line
	4950 5900 4950 5800
Connection ~ 4950 5800
Wire Wire Line
	6150 5800 6250 5800
Wire Wire Line
	6250 5800 6250 5850
$Comp
L SamacSys_Parts:DM3AT-SF-PEJM5 J2
U 1 1 5B4F3341
P 4100 3400
F 0 "J2" H 7150 3700 50  0000 L CNN
F 1 "DM3AT-SF-PEJM5" H 7150 3600 50  0000 L CNN
F 2 "SamacSys_Parts:DM3AT-SF-PEJM5_1" H 7150 3500 50  0001 L CNN
F 3 "http://uk.rs-online.com/web/p/products/1722520" H 7150 3400 50  0001 L CNN
F 4 "DM3 RA SMT microSD memory card push/push DM3 RA SMT microSD memory card push/push" H 7150 3300 50  0001 L CNN "Description"
F 5 "" H 7150 3200 50  0001 L CNN "Height"
F 6 "1722520" H 7150 3100 50  0001 L CNN "RS Part Number"
F 7 "Hirose" H 7150 3000 50  0001 L CNN "Manufacturer_Name"
F 8 "DM3AT-SF-PEJM5" H 7150 2900 50  0001 L CNN "Manufacturer_Part_Number"
	1    4100 3400
	1    0    0    -1  
$EndComp
NoConn ~ 4100 3400
Wire Wire Line
	3900 3450 3950 3450
Wire Wire Line
	3950 3450 3950 3500
Wire Wire Line
	3950 3500 4100 3500
Wire Wire Line
	3900 3600 4100 3600
Wire Wire Line
	7300 3400 7400 3400
Wire Wire Line
	7400 3400 7400 3350
Wire Wire Line
	7400 3350 7450 3350
Wire Wire Line
	7300 3500 8150 3500
Wire Wire Line
	8150 3500 8150 3400
Wire Wire Line
	4100 3700 3950 3700
Wire Wire Line
	3950 3700 3950 3750
Wire Wire Line
	3950 3750 3900 3750
Wire Wire Line
	7300 3800 7450 3800
Wire Wire Line
	7300 3600 8150 3600
Wire Wire Line
	8150 3600 8150 3650
$Comp
L power:Earth #PWR018
U 1 1 5B4F66FD
P 4000 3850
F 0 "#PWR018" H 4000 3600 50  0001 C CNN
F 1 "Earth" H 4000 3700 50  0001 C CNN
F 2 "" H 4000 3850 50  0000 C CNN
F 3 "" H 4000 3850 50  0000 C CNN
	1    4000 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 3800 4000 3800
Wire Wire Line
	4000 3800 4000 3850
NoConn ~ 7300 3700
$Comp
L User_Library:TEENSY_3.2_BASIC_33-PIN U1
U 1 1 5B5C99E9
P 1950 1550
F 0 "U1" H 1650 2450 60  0000 C CNN
F 1 "TEENSY_3.2_BASIC_33-PIN" H 2000 2350 60  0000 C CNN
F 2 "User Libraries:TEENSY_3.2_33-PIN" H 1950 1350 60  0001 C CNN
F 3 "" H 1950 1350 60  0001 C CNN
	1    1950 1550
	1    0    0    -1  
$EndComp
NoConn ~ 1850 2700
NoConn ~ 2050 2700
NoConn ~ 2150 2700
Wire Wire Line
	1300 6400 1200 6400
$Comp
L power:Earth #PWR?
U 1 1 5B5CB1B9
P 1950 2900
F 0 "#PWR?" H 1950 2650 50  0001 C CNN
F 1 "Earth" H 1950 2750 50  0001 C CNN
F 2 "" H 1950 2900 50  0000 C CNN
F 3 "" H 1950 2900 50  0000 C CNN
	1    1950 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 2700 1950 2900
Wire Wire Line
	9000 5450 8900 5450
Wire Wire Line
	2200 5600 2200 5800
Wire Wire Line
	3000 6100 3100 6100
Wire Wire Line
	3600 6450 3750 6450
Wire Wire Line
	5500 850  5550 850 
Wire Wire Line
	5950 1300 5950 1350
Wire Wire Line
	4950 5800 5050 5800
$EndSCHEMATC
