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
L Connector_Generic:Conn_01x02 J1
U 1 1 61DC83BA
P 4150 2900
F 0 "J1" H 4150 3000 50  0000 C CNN
F 1 "From 12 V DC source" H 4150 3100 50  0000 C CNN
F 2 "" H 4150 2900 50  0001 C CNN
F 3 "~" H 4150 2900 50  0001 C CNN
	1    4150 2900
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 61DC89E5
P 5900 2900
F 0 "J3" H 5850 3000 50  0000 L CNN
F 1 "To actuator coil" H 5850 3100 50  0000 L CNN
F 2 "" H 5900 2900 50  0001 C CNN
F 3 "~" H 5900 2900 50  0001 C CNN
	1    5900 2900
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 61DCA40E
P 4150 3600
F 0 "J2" H 4150 3700 50  0000 C CNN
F 1 "From MC" H 4150 3800 50  0000 C CNN
F 2 "" H 4150 3600 50  0001 C CNN
F 3 "~" H 4150 3600 50  0001 C CNN
	1    4150 3600
	-1   0    0    -1  
$EndComp
$Comp
L Device:D D1
U 1 1 61DCBAFF
P 5350 3100
F 0 "D1" V 5304 3180 50  0000 L CNN
F 1 "PG208 (or any 1-2 A)" V 5050 2650 50  0000 L CNN
F 2 "" H 5350 3100 50  0001 C CNN
F 3 "~" H 5350 3100 50  0001 C CNN
	1    5350 3100
	0    1    1    0   
$EndComp
$Comp
L Transistor_BJT:TIP120 Q1
U 1 1 61DCC71F
P 5250 3600
F 0 "Q1" H 5457 3646 50  0000 L CNN
F 1 "TIP120 (or any NPN Darlington Transistor > 1A)" H 5457 3555 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 5450 3525 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/TIP120-D.PDF" H 5250 3600 50  0001 L CNN
	1    5250 3600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 61DCDC43
P 4700 3600
F 0 "R1" V 4493 3600 50  0000 C CNN
F 1 "1k" V 4584 3600 50  0000 C CNN
F 2 "" V 4630 3600 50  0001 C CNN
F 3 "~" H 4700 3600 50  0001 C CNN
	1    4700 3600
	0    1    1    0   
$EndComp
Wire Wire Line
	4350 2900 4450 2900
Wire Wire Line
	5350 2950 5350 2900
Connection ~ 5350 2900
Wire Wire Line
	5350 2900 5600 2900
Wire Wire Line
	5350 3400 5350 3300
Wire Wire Line
	5350 3300 5650 3300
Wire Wire Line
	5650 3300 5650 3150
Wire Wire Line
	5650 3000 5700 3000
Connection ~ 5350 3300
Wire Wire Line
	5350 3300 5350 3250
Wire Wire Line
	5050 3600 4850 3600
Wire Wire Line
	4550 3600 4450 3600
Wire Wire Line
	4350 3700 4450 3700
Wire Wire Line
	4750 3700 4750 3850
Wire Wire Line
	4750 3850 4950 3850
Wire Wire Line
	5350 3850 5350 3800
Wire Wire Line
	4350 3000 4450 3000
Wire Wire Line
	4950 3000 4950 3850
Connection ~ 4950 3850
Wire Wire Line
	4950 3850 5350 3850
$Comp
L power:GND #PWR?
U 1 1 61DD76BA
P 4950 3950
F 0 "#PWR?" H 4950 3700 50  0001 C CNN
F 1 "GND" H 4955 3777 50  0000 C CNN
F 2 "" H 4950 3950 50  0001 C CNN
F 3 "" H 4950 3950 50  0001 C CNN
	1    4950 3950
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR?
U 1 1 61DD7AAA
P 4600 2800
F 0 "#PWR?" H 4600 2650 50  0001 C CNN
F 1 "+12V" H 4615 2973 50  0000 C CNN
F 2 "" H 4600 2800 50  0001 C CNN
F 3 "" H 4600 2800 50  0001 C CNN
	1    4600 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 2800 4600 2900
Connection ~ 4600 2900
Wire Wire Line
	4600 2900 5350 2900
Wire Wire Line
	4950 3850 4950 3950
Text GLabel 4000 3700 0    50   Input ~ 0
GND
Text GLabel 4000 3600 0    50   Input ~ 0
PA02
Text GLabel 4000 3000 0    50   Input ~ 0
GND
Text GLabel 4000 2900 0    50   Input ~ 0
+12V
Text GLabel 6050 2900 2    50   Input ~ 0
coil+
Text GLabel 6050 3000 2    50   Input ~ 0
coil-
Wire Wire Line
	4000 2900 4050 2900
Wire Wire Line
	4050 2900 4050 2750
Wire Wire Line
	4450 2750 4450 2900
Wire Wire Line
	4050 2750 4450 2750
Connection ~ 4450 2900
Wire Wire Line
	4450 2900 4600 2900
Wire Wire Line
	4450 3000 4450 3150
Wire Wire Line
	4450 3150 4050 3150
Wire Wire Line
	4050 3150 4050 3000
Wire Wire Line
	4050 3000 4000 3000
Connection ~ 4450 3000
Wire Wire Line
	4450 3000 4950 3000
Wire Wire Line
	4000 3600 4050 3600
Wire Wire Line
	4050 3600 4050 3450
Wire Wire Line
	4050 3450 4450 3450
Wire Wire Line
	4450 3450 4450 3600
Connection ~ 4450 3600
Wire Wire Line
	4450 3600 4350 3600
Wire Wire Line
	4450 3700 4450 3850
Wire Wire Line
	4450 3850 4050 3850
Wire Wire Line
	4050 3850 4050 3700
Wire Wire Line
	4050 3700 4000 3700
Connection ~ 4450 3700
Wire Wire Line
	4450 3700 4750 3700
Wire Wire Line
	5600 2900 5600 2750
Wire Wire Line
	5600 2750 6000 2750
Wire Wire Line
	6000 2750 6000 2900
Wire Wire Line
	6000 2900 6050 2900
Connection ~ 5600 2900
Wire Wire Line
	5600 2900 5700 2900
Wire Wire Line
	6050 3000 6000 3000
Wire Wire Line
	6000 3000 6000 3150
Wire Wire Line
	6000 3150 5650 3150
Connection ~ 5650 3150
Wire Wire Line
	5650 3150 5650 3000
$EndSCHEMATC
