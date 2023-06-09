EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L micro:ProMicro U1
U 1 1 63FB594D
P 3200 3500
F 0 "U1" H 3200 4537 60  0000 C CNN
F 1 "ProMicro" H 3200 4431 60  0000 C CNN
F 2 "micro:ProMicro-NoSilk" H 3300 2450 60  0001 C CNN
F 3 "" H 3300 2450 60  0000 C CNN
	1    3200 3500
	1    0    0    -1  
$EndComp
$Comp
L RF_Module:Ai-Thinker-Ra-01 U2
U 1 1 63FB75DB
P 7550 3300
F 0 "U2" H 7550 4281 50  0000 C CNN
F 1 "Ai-Thinker-Ra-01" H 7550 4190 50  0000 C CNN
F 2 "RF_Module:Ai-Thinker-Ra-01-LoRa" H 8550 2900 50  0001 C CNN
F 3 "https://mikroelectron.com/Product/10KM-433M-LORA-LONG-RANGE-WIRELESS-MODULE-RA-01" H 7650 4000 50  0001 C CNN
	1    7550 3300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 63FBB75A
P 7550 4000
F 0 "#PWR04" H 7550 3750 50  0001 C CNN
F 1 "GND" H 7555 3827 50  0000 C CNN
F 2 "" H 7550 4000 50  0001 C CNN
F 3 "" H 7550 4000 50  0001 C CNN
	1    7550 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 63FBBE18
P 4300 2900
F 0 "#PWR02" H 4300 2650 50  0001 C CNN
F 1 "GND" H 4305 2727 50  0000 C CNN
F 2 "" H 4300 2900 50  0001 C CNN
F 3 "" H 4300 2900 50  0001 C CNN
	1    4300 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 3050 2200 3050
Wire Wire Line
	2500 2950 2200 2950
Wire Wire Line
	2200 2950 2200 3000
Text GLabel 3900 3750 2    50   Input ~ 0
MOSI
Text GLabel 3900 3650 2    50   Input ~ 0
MISO
Text GLabel 3900 3550 2    50   Input ~ 0
SCK
Text GLabel 7050 3500 0    50   Input ~ 0
MOSI
Text GLabel 7050 3400 0    50   Input ~ 0
MISO
Text GLabel 7050 3300 0    50   Input ~ 0
SCK
$Comp
L power:+3.3V #PWR03
U 1 1 63FC4C72
P 7550 2050
F 0 "#PWR03" H 7550 1900 50  0001 C CNN
F 1 "+3.3V" H 7565 2223 50  0000 C CNN
F 2 "" H 7550 2050 50  0001 C CNN
F 3 "" H 7550 2050 50  0001 C CNN
	1    7550 2050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR01
U 1 1 63FC6B03
P 4150 2750
F 0 "#PWR01" H 4150 2600 50  0001 C CNN
F 1 "+3.3V" H 4165 2923 50  0000 C CNN
F 2 "" H 4150 2750 50  0001 C CNN
F 3 "" H 4150 2750 50  0001 C CNN
	1    4150 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 2750 4150 3050
Wire Wire Line
	4150 3050 3900 3050
Text GLabel 7050 3200 0    50   Input ~ 0
NSS
Text GLabel 7050 3000 0    50   Input ~ 0
NRST
Text GLabel 8050 2900 2    50   Input ~ 0
DIO0
Text GLabel 8050 3000 2    50   Input ~ 0
DIO1
NoConn ~ 8050 3100
NoConn ~ 8050 3200
NoConn ~ 8050 3300
NoConn ~ 8050 3400
Text GLabel 2500 3450 0    50   Input ~ 0
NSS
Text GLabel 2500 3350 0    50   Input ~ 0
NRST
Text GLabel 2500 3250 0    50   Input ~ 0
DIO0
Text GLabel 2500 3150 0    50   Input ~ 0
DIO1
NoConn ~ 2500 3850
NoConn ~ 2500 3750
NoConn ~ 2500 3650
NoConn ~ 2500 3550
NoConn ~ 2500 2850
NoConn ~ 2500 2750
NoConn ~ 3900 2950
NoConn ~ 3900 2750
$Comp
L Device:C C3
U 1 1 63FE046E
P 5950 5600
F 0 "C3" H 6065 5646 50  0000 L CNN
F 1 "100nF" H 6065 5555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 5988 5450 50  0001 C CNN
F 3 "~" H 5950 5600 50  0001 C CNN
	1    5950 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 5800 4900 5700
Wire Wire Line
	5400 5400 5400 5100
Wire Wire Line
	5400 5100 5150 5100
Wire Wire Line
	5400 5700 5400 5800
Wire Wire Line
	5400 5800 5150 5800
NoConn ~ 3900 3850
$Comp
L power:+3.3V #PWR05
U 1 1 63FE9D7C
P 1800 5350
F 0 "#PWR05" H 1800 5200 50  0001 C CNN
F 1 "+3.3V" H 1815 5523 50  0000 C CNN
F 2 "" H 1800 5350 50  0001 C CNN
F 3 "" H 1800 5350 50  0001 C CNN
	1    1800 5350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 63FEB458
P 1800 5650
F 0 "#PWR06" H 1800 5400 50  0001 C CNN
F 1 "GND" H 1805 5477 50  0000 C CNN
F 2 "" H 1800 5650 50  0001 C CNN
F 3 "" H 1800 5650 50  0001 C CNN
	1    1800 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 5450 1800 5350
Wire Wire Line
	1800 5550 1800 5650
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 63FF05B4
P 1600 5450
F 0 "J1" H 1708 5631 50  0000 C CNN
F 1 "Conn_01x02_Male" H 1708 5540 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 1600 5450 50  0001 C CNN
F 3 "~" H 1600 5450 50  0001 C CNN
	1    1600 5450
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_Coaxial J2
U 1 1 63FF50A8
P 7400 5250
F 0 "J2" V 7637 5179 50  0000 C CNN
F 1 "Conn_Coaxial" V 7546 5179 50  0000 C CNN
F 2 "Connector_Coaxial:SMA_Samtec_SMA-J-P-X-ST-EM1_EdgeMount" H 7400 5250 50  0001 C CNN
F 3 " ~" H 7400 5250 50  0001 C CNN
	1    7400 5250
	0    -1   -1   0   
$EndComp
NoConn ~ 3900 3250
NoConn ~ 3900 3350
NoConn ~ 3900 3450
$Comp
L Device:R R1
U 1 1 640A6314
P 3050 5250
F 0 "R1" H 3120 5296 50  0000 L CNN
F 1 "20k" H 3120 5205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 2980 5250 50  0001 C CNN
F 3 "~" H 3050 5250 50  0001 C CNN
	1    3050 5250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 640A676D
P 3050 5700
F 0 "R2" H 3120 5746 50  0000 L CNN
F 1 "20k" H 3120 5655 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 2980 5700 50  0001 C CNN
F 3 "~" H 3050 5700 50  0001 C CNN
	1    3050 5700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 640A9B90
P 3050 6050
F 0 "#PWR0101" H 3050 5800 50  0001 C CNN
F 1 "GND" H 3055 5877 50  0000 C CNN
F 2 "" H 3050 6050 50  0001 C CNN
F 3 "" H 3050 6050 50  0001 C CNN
	1    3050 6050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0102
U 1 1 640AA637
P 3050 4900
F 0 "#PWR0102" H 3050 4750 50  0001 C CNN
F 1 "+3.3V" H 3065 5073 50  0000 C CNN
F 2 "" H 3050 4900 50  0001 C CNN
F 3 "" H 3050 4900 50  0001 C CNN
	1    3050 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 4900 3050 5100
Wire Wire Line
	3050 5400 3050 5450
Wire Wire Line
	3050 5850 3050 6050
Text GLabel 3900 3150 2    50   Input ~ 0
VBAT
Text GLabel 3500 5450 2    50   Input ~ 0
VBAT
Wire Wire Line
	3500 5450 3050 5450
Connection ~ 3050 5450
Wire Wire Line
	3050 5450 3050 5550
$Comp
L power:+3.3V #PWR0103
U 1 1 640BFA9B
P 5150 4950
F 0 "#PWR0103" H 5150 4800 50  0001 C CNN
F 1 "+3.3V" H 5165 5123 50  0000 C CNN
F 2 "" H 5150 4950 50  0001 C CNN
F 3 "" H 5150 4950 50  0001 C CNN
	1    5150 4950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 640C0E1E
P 5150 5950
F 0 "#PWR0104" H 5150 5700 50  0001 C CNN
F 1 "GND" H 5155 5777 50  0000 C CNN
F 2 "" H 5150 5950 50  0001 C CNN
F 3 "" H 5150 5950 50  0001 C CNN
	1    5150 5950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 640C1EA4
P 2000 3000
F 0 "#PWR0105" H 2000 2750 50  0001 C CNN
F 1 "GND" H 2005 2827 50  0000 C CNN
F 2 "" H 2000 3000 50  0001 C CNN
F 3 "" H 2000 3000 50  0001 C CNN
	1    2000 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 5950 5150 5800
Connection ~ 5150 5800
Wire Wire Line
	5150 5800 4900 5800
Wire Wire Line
	5150 4950 5150 5100
Connection ~ 5150 5100
Wire Wire Line
	5150 5100 4900 5100
Wire Wire Line
	3900 2850 4300 2850
Wire Wire Line
	4300 2850 4300 2900
Wire Wire Line
	2200 3000 2000 3000
Connection ~ 2200 3000
Wire Wire Line
	2200 3000 2200 3050
$Comp
L Device:CP C2
U 1 1 63FE2CEF
P 5400 5550
F 0 "C2" H 5518 5596 50  0000 L CNN
F 1 "47uF" H 5518 5505 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-7132-28_AVX-C_Pad2.72x3.50mm_HandSolder" H 5438 5400 50  0001 C CNN
F 3 "~" H 5400 5550 50  0001 C CNN
	1    5400 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 5100 4900 5400
$Comp
L Device:C C1
U 1 1 63FE1467
P 4900 5550
F 0 "C1" H 5015 5596 50  0000 L CNN
F 1 "100nF" H 5015 5505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 4938 5400 50  0001 C CNN
F 3 "~" H 4900 5550 50  0001 C CNN
	1    4900 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 5800 5950 5800
Wire Wire Line
	5950 5800 5950 5750
Connection ~ 5400 5800
Wire Wire Line
	5950 5450 5950 5100
Wire Wire Line
	5950 5100 5400 5100
Connection ~ 5400 5100
Wire Wire Line
	7550 2050 7550 2500
Text GLabel 7050 2800 0    50   Input ~ 0
ANT
Text GLabel 7400 5450 3    50   Input ~ 0
ANT
$Comp
L power:GND #PWR0106
U 1 1 640D71DF
P 7600 5250
F 0 "#PWR0106" H 7600 5000 50  0001 C CNN
F 1 "GND" H 7605 5077 50  0000 C CNN
F 2 "" H 7600 5250 50  0001 C CNN
F 3 "" H 7600 5250 50  0001 C CNN
	1    7600 5250
	1    0    0    -1  
$EndComp
$EndSCHEMATC
