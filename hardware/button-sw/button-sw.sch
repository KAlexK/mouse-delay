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
L Isolator:CNY17-1 U1
U 1 1 61DDC7EC
P 5650 3500
F 0 "U1" H 5650 3700 50  0000 C CNN
F 1 "CNY17 (or any optocoupler with phototransistor output)" H 7150 3500 50  0000 C CNN
F 2 "" H 5650 3500 50  0001 L CNN
F 3 "http://www.vishay.com/docs/83606/cny17.pdf" H 5650 3500 50  0001 L CNN
	1    5650 3500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 61DDE7B1
P 5600 4350
F 0 "J1" V 5600 4150 50  0000 C CNN
F 1 "-" H 5600 4150 50  0001 C CNN
F 2 "" H 5600 4350 50  0001 C CNN
F 3 "~" H 5600 4350 50  0001 C CNN
	1    5600 4350
	0    -1   1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 61DDF332
P 5300 3800
F 0 "R1" V 5200 3800 50  0000 C CNN
F 1 "100" V 5300 3800 50  0000 C CNN
F 2 "" V 5230 3800 50  0001 C CNN
F 3 "~" H 5300 3800 50  0001 C CNN
	1    5300 3800
	-1   0    0    1   
$EndComp
Text GLabel 5550 3150 0    50   Input ~ 0
+3.3V
Text GLabel 5550 4100 0    50   Input ~ 0
PA02
Text GLabel 5750 4100 2    50   Input ~ 0
Button-
Text GLabel 5750 3150 2    50   Input ~ 0
Button+
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 61DDD89B
P 5600 2900
F 0 "J2" V 5600 2950 50  0000 L CNN
F 1 "+" H 5450 3150 50  0001 L CNN
F 2 "" H 5600 2900 50  0001 C CNN
F 3 "~" H 5600 2900 50  0001 C CNN
	1    5600 2900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5700 4000 6000 4000
Wire Wire Line
	6000 4000 6000 3600
Wire Wire Line
	6000 3600 5950 3600
Wire Wire Line
	5950 3500 6000 3500
Wire Wire Line
	6000 3500 6000 3250
Wire Wire Line
	6000 3250 5700 3250
Wire Wire Line
	5350 3400 5300 3400
Wire Wire Line
	5300 3400 5300 3250
Wire Wire Line
	5300 3250 5600 3250
Wire Wire Line
	5350 3600 5300 3600
Wire Wire Line
	5300 3600 5300 3650
Wire Wire Line
	5300 3950 5300 4000
Wire Wire Line
	5300 4000 5600 4000
Wire Wire Line
	5700 3100 5700 3150
Wire Wire Line
	5600 3100 5600 3150
Wire Wire Line
	5700 4000 5700 4100
Wire Wire Line
	5600 4000 5600 4100
Wire Wire Line
	5750 3150 5700 3150
Connection ~ 5700 3150
Wire Wire Line
	5700 3150 5700 3250
Wire Wire Line
	5550 3150 5600 3150
Connection ~ 5600 3150
Wire Wire Line
	5600 3150 5600 3250
Wire Wire Line
	5750 4100 5700 4100
Connection ~ 5700 4100
Wire Wire Line
	5700 4100 5700 4150
Wire Wire Line
	5550 4100 5600 4100
Connection ~ 5600 4100
Wire Wire Line
	5600 4100 5600 4150
$EndSCHEMATC
