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
L Primavera_Monitor-rescue:BackBoard-MSP-EXP432P401R U?
U 1 1 615EADFD
P 5450 3750
F 0 "U?" H 4900 5000 50  0000 C CNN
F 1 "BackBoard" H 5050 5100 50  0000 C CNN
F 2 "" H 4850 2800 50  0001 C CNN
F 3 "" H 4850 2800 50  0001 C CNN
	1    5450 3750
	1    0    0    -1  
$EndComp
$Comp
L Primavera_Monitor-rescue:RTC_DS3231_Mod-RTC_DS3231 U?
U 1 1 615EE710
P 5600 6750
F 0 "U?" H 5400 7200 50  0000 C CNN
F 1 "RTC_DS3231_Mod" H 5150 7300 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x06_P2.54mm_Vertical" H 5550 5850 50  0001 C CNN
F 3 "" H 5050 7250 50  0001 C CNN
	1    5600 6750
	-1   0    0    -1  
$EndComp
$Comp
L Primavera_Monitor-rescue:MPU6050_Mod-MPU6050 U?
U 1 1 615F19F9
P 9850 3600
F 0 "U?" H 10000 2950 50  0000 C CNN
F 1 "MPU6050_Mod" H 10200 3050 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 9850 4550 50  0001 C CNN
F 3 "" H 9700 2550 50  0001 C CNN
	1    9850 3600
	1    0    0    -1  
$EndComp
$Comp
L Primavera_Monitor-rescue:BME280_Mod_LC-BME280 U?
U 1 1 615F30C2
P 9800 1800
F 0 "U?" H 10078 1891 50  0000 L CNN
F 1 "BME280_Mod_LC" H 10078 1800 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 11350 1700 50  0001 C CNN
F 3 "https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf" H 9550 1850 50  0001 C CNN
F 4 "Tinytronics" H 10078 1709 50  0000 L CNN "Manufacturer"
	1    9800 1800
	1    0    0    -1  
$EndComp
$Comp
L Primavera_Monitor-rescue:SD_Card_Interface-SD_Card_Interface U?
U 1 1 615F4E52
P 1550 6850
F 0 "U?" H 1878 6896 50  0000 L CNN
F 1 "SD_Card_Interface" H 1878 6805 50  0000 L CNN
F 2 "" H 1750 7500 50  0001 C CNN
F 3 "" H 1750 7500 50  0001 C CNN
	1    1550 6850
	-1   0    0    -1  
$EndComp
$Comp
L Primavera_Monitor-rescue:OPB720b_mod-OPB720b U?
U 1 1 615F706E
P 1550 5150
F 0 "U?" H 1222 5196 50  0000 R CNN
F 1 "OPB720b_mod" H 1222 5105 50  0000 R CNN
F 2 "" H 1100 5550 50  0001 C CNN
F 3 "" H 1100 5550 50  0001 C CNN
	1    1550 5150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 615FF6A9
P 9850 6050
F 0 "#PWR?" H 9850 5800 50  0001 C CNN
F 1 "GND" H 9855 5877 50  0000 C CNN
F 2 "" H 9850 6050 50  0001 C CNN
F 3 "" H 9850 6050 50  0001 C CNN
	1    9850 6050
	1    0    0    -1  
$EndComp
$Comp
L Primavera_Monitor-rescue:MPU6050_Mod-MPU6050 U?
U 1 1 615F00F1
P 9850 5400
F 0 "U?" H 10050 4750 50  0000 C CNN
F 1 "MPU6050_Mod" H 10250 4850 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 9850 6350 50  0001 C CNN
F 3 "" H 9700 4350 50  0001 C CNN
	1    9850 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 615FF408
P 9850 4250
F 0 "#PWR?" H 9850 4000 50  0001 C CNN
F 1 "GND" H 9855 4077 50  0000 C CNN
F 2 "" H 9850 4250 50  0001 C CNN
F 3 "" H 9850 4250 50  0001 C CNN
	1    9850 4250
	1    0    0    -1  
$EndComp
Text GLabel 9150 3450 0    50   Input ~ 0
I2C_2_SCL
Text GLabel 9150 5250 0    50   Input ~ 0
I2C_2_SCL
Text GLabel 9150 5400 0    50   Input ~ 0
I2C_2_SDA
Text GLabel 9150 3600 0    50   Input ~ 0
I2C_2_SDA
Wire Wire Line
	9150 3450 9300 3450
Wire Wire Line
	9150 3600 9300 3600
Wire Wire Line
	9150 5250 9300 5250
Wire Wire Line
	9150 5400 9300 5400
Wire Wire Line
	9850 5950 9850 6050
Wire Wire Line
	9850 4150 9850 4250
$Comp
L power:GND #PWR?
U 1 1 61616C19
P 9850 2250
F 0 "#PWR?" H 9850 2000 50  0001 C CNN
F 1 "GND" H 9855 2077 50  0000 C CNN
F 2 "" H 9850 2250 50  0001 C CNN
F 3 "" H 9850 2250 50  0001 C CNN
	1    9850 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 2150 9850 2250
$Comp
L power:+3V3 #PWR?
U 1 1 61618060
P 9850 1300
F 0 "#PWR?" H 9850 1150 50  0001 C CNN
F 1 "+3V3" H 9865 1473 50  0000 C CNN
F 2 "" H 9850 1300 50  0001 C CNN
F 3 "" H 9850 1300 50  0001 C CNN
	1    9850 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 1300 9850 1450
Text GLabel 9250 1750 0    50   Input ~ 0
I2C_1_SCL
Wire Wire Line
	9250 1750 9450 1750
Text GLabel 9250 1850 0    50   Input ~ 0
I2C_1_SDA
Wire Wire Line
	9250 1850 9450 1850
Text GLabel 2200 6650 2    50   Input ~ 0
SPI_CLK
Text GLabel 2200 6800 2    50   Input ~ 0
SPI_Tx
Text GLabel 2200 6900 2    50   Input ~ 0
SPI_Rx
Text GLabel 2200 7050 2    50   Input ~ 0
SPI_CS
Wire Wire Line
	2200 6650 2000 6650
Wire Wire Line
	2200 6800 2000 6800
Wire Wire Line
	2200 6900 2000 6900
Wire Wire Line
	2200 7050 2000 7050
$Comp
L power:+3V3 #PWR?
U 1 1 61639C7D
P 1550 6300
F 0 "#PWR?" H 1550 6150 50  0001 C CNN
F 1 "+3V3" H 1565 6473 50  0000 C CNN
F 2 "" H 1550 6300 50  0001 C CNN
F 3 "" H 1550 6300 50  0001 C CNN
	1    1550 6300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 6300 1550 6450
$Comp
L power:GND #PWR?
U 1 1 6163A8EE
P 1400 7400
F 0 "#PWR?" H 1400 7150 50  0001 C CNN
F 1 "GND" H 1405 7227 50  0000 C CNN
F 2 "" H 1400 7400 50  0001 C CNN
F 3 "" H 1400 7400 50  0001 C CNN
	1    1400 7400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 7250 1400 7350
Wire Wire Line
	1550 7250 1550 7350
Wire Wire Line
	1550 7350 1400 7350
Connection ~ 1400 7350
Wire Wire Line
	1400 7350 1400 7400
$Comp
L power:GND #PWR?
U 1 1 6163C47E
P 1550 5650
F 0 "#PWR?" H 1550 5400 50  0001 C CNN
F 1 "GND" H 1555 5477 50  0000 C CNN
F 2 "" H 1550 5650 50  0001 C CNN
F 3 "" H 1550 5650 50  0001 C CNN
	1    1550 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 5500 1550 5650
Text GLabel 2250 5150 2    50   Input ~ 0
CCR
Text GLabel 1700 4700 2    50   Input ~ 0
PWM
$Comp
L power:GND #PWR?
U 1 1 61641677
P 5600 7350
F 0 "#PWR?" H 5600 7100 50  0001 C CNN
F 1 "GND" H 5605 7177 50  0000 C CNN
F 2 "" H 5600 7350 50  0001 C CNN
F 3 "" H 5600 7350 50  0001 C CNN
	1    5600 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 7200 5600 7350
Text GLabel 6200 6850 2    50   Input ~ 0
I2C_1_SDA
Text GLabel 6200 6650 2    50   Input ~ 0
I2C_1_SCL
Wire Wire Line
	6200 6650 6050 6650
Wire Wire Line
	6200 6850 6050 6850
$Comp
L power:+3V3 #PWR?
U 1 1 6164837C
P 5600 6100
F 0 "#PWR?" H 5600 5950 50  0001 C CNN
F 1 "+3V3" H 5615 6273 50  0000 C CNN
F 2 "" H 5600 6100 50  0001 C CNN
F 3 "" H 5600 6100 50  0001 C CNN
	1    5600 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 6100 5600 6300
Text GLabel 3600 3800 0    50   Input ~ 0
I2C_1_SCL
Text GLabel 3600 3700 0    50   Input ~ 0
I2C_1_SDA
Text GLabel 3250 3400 0    50   Input ~ 0
I2C_2_SDA
Text GLabel 3250 3500 0    50   Input ~ 0
I2C_2_SCL
Text GLabel 4500 3000 0    50   Input ~ 0
SPI_CLK
Text GLabel 4500 3100 0    50   Input ~ 0
SPI_Tx
Text GLabel 4500 3200 0    50   Input ~ 0
SPI_Rx
Text GLabel 3600 4000 0    50   Input ~ 0
SPI_CS
$Comp
L power:GND #PWR?
U 1 1 61652D6E
P 5450 5300
F 0 "#PWR?" H 5450 5050 50  0001 C CNN
F 1 "GND" H 5455 5127 50  0000 C CNN
F 2 "" H 5450 5300 50  0001 C CNN
F 3 "" H 5450 5300 50  0001 C CNN
	1    5450 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 5100 5450 5200
Wire Wire Line
	5650 5100 5650 5200
Wire Wire Line
	5650 5200 5450 5200
Connection ~ 5450 5200
Wire Wire Line
	5450 5200 5450 5300
Wire Wire Line
	3600 4000 4550 4000
Wire Wire Line
	3600 3700 3850 3700
Wire Wire Line
	3600 3800 3950 3800
Wire Wire Line
	4500 3200 4550 3200
Wire Wire Line
	4500 3100 4550 3100
Wire Wire Line
	4500 3000 4550 3000
$Comp
L Device:R R?
U 1 1 616731A8
P 3950 3100
F 0 "R?" H 4020 3146 50  0000 L CNN
F 1 "R" H 4020 3055 50  0000 L CNN
F 2 "" V 3880 3100 50  0001 C CNN
F 3 "~" H 3950 3100 50  0001 C CNN
	1    3950 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 3250 3950 3800
Connection ~ 3950 3800
Wire Wire Line
	3950 3800 4550 3800
$Comp
L Device:R R?
U 1 1 6167452D
P 3850 3100
F 0 "R?" H 3700 3150 50  0000 L CNN
F 1 "R" H 3700 3050 50  0000 L CNN
F 2 "" V 3780 3100 50  0001 C CNN
F 3 "~" H 3850 3100 50  0001 C CNN
	1    3850 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 616747AA
P 3500 3100
F 0 "R?" H 3570 3146 50  0000 L CNN
F 1 "R" H 3570 3055 50  0000 L CNN
F 2 "" V 3430 3100 50  0001 C CNN
F 3 "~" H 3500 3100 50  0001 C CNN
	1    3500 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 3250 3850 3700
Connection ~ 3850 3700
Wire Wire Line
	3850 3700 4550 3700
$Comp
L Device:R R?
U 1 1 6167FAB6
P 3400 3100
F 0 "R?" H 3250 3150 50  0000 L CNN
F 1 "R" H 3250 3050 50  0000 L CNN
F 2 "" V 3330 3100 50  0001 C CNN
F 3 "~" H 3400 3100 50  0001 C CNN
	1    3400 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 3250 3500 3500
Wire Wire Line
	3500 3500 4550 3500
Wire Wire Line
	3400 3250 3400 3400
Wire Wire Line
	3400 3400 4550 3400
Wire Wire Line
	3250 3400 3400 3400
Connection ~ 3400 3400
Wire Wire Line
	3250 3500 3500 3500
Connection ~ 3500 3500
$Comp
L power:+3V3 #PWR?
U 1 1 6168649D
P 3450 2800
F 0 "#PWR?" H 3450 2650 50  0001 C CNN
F 1 "+3V3" H 3465 2973 50  0000 C CNN
F 2 "" H 3450 2800 50  0001 C CNN
F 3 "" H 3450 2800 50  0001 C CNN
	1    3450 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 2800 3450 2850
Wire Wire Line
	3450 2850 3400 2850
Wire Wire Line
	3400 2850 3400 2950
Wire Wire Line
	3450 2850 3500 2850
Wire Wire Line
	3500 2850 3500 2950
Connection ~ 3450 2850
$Comp
L power:+3V3 #PWR?
U 1 1 61688CEC
P 3900 2800
F 0 "#PWR?" H 3900 2650 50  0001 C CNN
F 1 "+3V3" H 3915 2973 50  0000 C CNN
F 2 "" H 3900 2800 50  0001 C CNN
F 3 "" H 3900 2800 50  0001 C CNN
	1    3900 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 2800 3900 2850
Wire Wire Line
	3900 2850 3850 2850
Wire Wire Line
	3850 2850 3850 2950
Wire Wire Line
	3900 2850 3950 2850
Wire Wire Line
	3950 2850 3950 2950
Connection ~ 3900 2850
$Comp
L power:GND #PWR?
U 1 1 6169CA79
P 9150 3900
F 0 "#PWR?" H 9150 3650 50  0001 C CNN
F 1 "GND" H 9155 3727 50  0000 C CNN
F 2 "" H 9150 3900 50  0001 C CNN
F 3 "" H 9150 3900 50  0001 C CNN
	1    9150 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 3750 9150 3750
Wire Wire Line
	9150 3750 9150 3900
$Comp
L power:+3V3 #PWR?
U 1 1 6169F89F
P 8500 5400
F 0 "#PWR?" H 8500 5250 50  0001 C CNN
F 1 "+3V3" H 8515 5573 50  0000 C CNN
F 2 "" H 8500 5400 50  0001 C CNN
F 3 "" H 8500 5400 50  0001 C CNN
	1    8500 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 5400 8500 5550
Wire Wire Line
	8500 5550 9300 5550
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 616D2CA2
P 1350 1600
F 0 "J?" H 1900 1600 50  0000 C CNN
F 1 "Conn_01x02" H 1750 1500 50  0000 C CNN
F 2 "" H 1350 1600 50  0001 C CNN
F 3 "~" H 1350 1600 50  0001 C CNN
	1    1350 1600
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 616D43B3
P 1700 1750
F 0 "#PWR?" H 1700 1500 50  0001 C CNN
F 1 "GND" H 1705 1577 50  0000 C CNN
F 2 "" H 1700 1750 50  0001 C CNN
F 3 "" H 1700 1750 50  0001 C CNN
	1    1700 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 1600 1700 1600
Wire Wire Line
	1700 1600 1700 1750
Wire Wire Line
	1700 1500 1550 1500
Wire Wire Line
	1700 4700 1550 4700
Wire Wire Line
	1550 4700 1550 4800
Wire Wire Line
	1950 5150 2150 5150
$Comp
L Device:R R?
U 1 1 616E2775
P 2150 4900
F 0 "R?" H 2220 4946 50  0000 L CNN
F 1 "R" H 2220 4855 50  0000 L CNN
F 2 "" V 2080 4900 50  0001 C CNN
F 3 "~" H 2150 4900 50  0001 C CNN
	1    2150 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 5050 2150 5150
Connection ~ 2150 5150
Wire Wire Line
	2150 5150 2250 5150
$Comp
L power:+3V3 #PWR?
U 1 1 616E44BA
P 2150 4600
F 0 "#PWR?" H 2150 4450 50  0001 C CNN
F 1 "+3V3" H 2165 4773 50  0000 C CNN
F 2 "" H 2150 4600 50  0001 C CNN
F 3 "" H 2150 4600 50  0001 C CNN
	1    2150 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 4600 2150 4750
$Comp
L power:+5V #PWR?
U 1 1 617E61EB
P 5650 1600
F 0 "#PWR?" H 5650 1450 50  0001 C CNN
F 1 "+5V" H 5665 1773 50  0000 C CNN
F 2 "" H 5650 1600 50  0001 C CNN
F 3 "" H 5650 1600 50  0001 C CNN
	1    5650 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 1600 5650 1750
$Comp
L Device:D D?
U 1 1 617E8D63
P 5250 1750
F 0 "D?" H 5250 1533 50  0000 C CNN
F 1 "D" H 5250 1624 50  0000 C CNN
F 2 "" H 5250 1750 50  0001 C CNN
F 3 "~" H 5250 1750 50  0001 C CNN
	1    5250 1750
	-1   0    0    1   
$EndComp
Wire Wire Line
	5400 1750 5650 1750
Connection ~ 5650 1750
Wire Wire Line
	5650 1750 5650 2400
$Comp
L Device:C C?
U 1 1 617EB750
P 2850 1600
F 0 "C?" H 2965 1646 50  0000 L CNN
F 1 "C" H 2965 1555 50  0000 L CNN
F 2 "" H 2888 1450 50  0001 C CNN
F 3 "~" H 2850 1600 50  0001 C CNN
	1    2850 1600
	1    0    0    -1  
$EndComp
$Comp
L Device:L L?
U 1 1 617EC61F
P 3300 1350
F 0 "L?" V 3490 1350 50  0000 C CNN
F 1 "L" V 3399 1350 50  0000 C CNN
F 2 "" H 3300 1350 50  0001 C CNN
F 3 "~" H 3300 1350 50  0001 C CNN
	1    3300 1350
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C?
U 1 1 617ED045
P 3750 1600
F 0 "C?" H 3865 1646 50  0000 L CNN
F 1 "C" H 3865 1555 50  0000 L CNN
F 2 "" H 3788 1450 50  0001 C CNN
F 3 "~" H 3750 1600 50  0001 C CNN
	1    3750 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 1350 4400 1750
Wire Wire Line
	4400 1750 5100 1750
Wire Wire Line
	3450 1350 3750 1350
Wire Wire Line
	3750 1450 3750 1350
Connection ~ 3750 1350
Wire Wire Line
	3750 1350 4400 1350
Wire Wire Line
	3150 1350 2850 1350
Wire Wire Line
	1700 1350 1700 1500
Wire Wire Line
	2850 1450 2850 1350
Connection ~ 2850 1350
Wire Wire Line
	2850 1350 1700 1350
$Comp
L power:GND #PWR?
U 1 1 617F6668
P 2850 1900
F 0 "#PWR?" H 2850 1650 50  0001 C CNN
F 1 "GND" H 2855 1727 50  0000 C CNN
F 2 "" H 2850 1900 50  0001 C CNN
F 3 "" H 2850 1900 50  0001 C CNN
	1    2850 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 1750 2850 1900
$Comp
L power:GND #PWR?
U 1 1 617F9084
P 3750 1900
F 0 "#PWR?" H 3750 1650 50  0001 C CNN
F 1 "GND" H 3755 1727 50  0000 C CNN
F 2 "" H 3750 1900 50  0001 C CNN
F 3 "" H 3750 1900 50  0001 C CNN
	1    3750 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 1750 3750 1900
$Comp
L Regulator_Linear:IFX27001TFV33 U?
U 1 1 617FE089
P 6400 1750
F 0 "U?" H 6400 2017 50  0000 C CNN
F 1 "IFX27001TFV33" H 6400 1926 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-252-3_TabPin2" H 6400 1700 50  0001 C CNN
F 3 "https://static6.arrow.com/aropdfconversion/dc75757ae45a88e5f69bdce3f2a651a5fe0ca07d/ifx27001_ds_10.pdf" H 6400 1700 50  0001 C CNN
	1    6400 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 617FEA2E
P 5900 1950
F 0 "C?" H 6015 1996 50  0000 L CNN
F 1 "C" H 6015 1905 50  0000 L CNN
F 2 "" H 5938 1800 50  0001 C CNN
F 3 "~" H 5900 1950 50  0001 C CNN
	1    5900 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 617FEF8A
P 6900 1950
F 0 "C?" H 7015 1996 50  0000 L CNN
F 1 "C" H 7015 1905 50  0000 L CNN
F 2 "" H 6938 1800 50  0001 C CNN
F 3 "~" H 6900 1950 50  0001 C CNN
	1    6900 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 1750 5900 1750
Wire Wire Line
	5900 1800 5900 1750
Connection ~ 5900 1750
Wire Wire Line
	5900 1750 6100 1750
Wire Wire Line
	6700 1750 6900 1750
Wire Wire Line
	6900 1800 6900 1750
Connection ~ 6900 1750
Wire Wire Line
	6900 1750 7050 1750
$Comp
L power:GND #PWR?
U 1 1 6180941E
P 5900 2200
F 0 "#PWR?" H 5900 1950 50  0001 C CNN
F 1 "GND" H 5905 2027 50  0000 C CNN
F 2 "" H 5900 2200 50  0001 C CNN
F 3 "" H 5900 2200 50  0001 C CNN
	1    5900 2200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 618099FD
P 6400 2200
F 0 "#PWR?" H 6400 1950 50  0001 C CNN
F 1 "GND" H 6405 2027 50  0000 C CNN
F 2 "" H 6400 2200 50  0001 C CNN
F 3 "" H 6400 2200 50  0001 C CNN
	1    6400 2200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61809F24
P 6900 2200
F 0 "#PWR?" H 6900 1950 50  0001 C CNN
F 1 "GND" H 6905 2027 50  0000 C CNN
F 2 "" H 6900 2200 50  0001 C CNN
F 3 "" H 6900 2200 50  0001 C CNN
	1    6900 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 2100 5900 2200
Wire Wire Line
	6400 2050 6400 2200
Wire Wire Line
	6900 2100 6900 2200
$Comp
L power:+3V3 #PWR?
U 1 1 61812BD2
P 7050 1550
F 0 "#PWR?" H 7050 1400 50  0001 C CNN
F 1 "+3V3" H 7065 1723 50  0000 C CNN
F 2 "" H 7050 1550 50  0001 C CNN
F 3 "" H 7050 1550 50  0001 C CNN
	1    7050 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 1550 7050 1750
$Comp
L power:+5V #PWR?
U 1 1 6181BF4F
P 9850 2850
F 0 "#PWR?" H 9850 2700 50  0001 C CNN
F 1 "+5V" H 9865 3023 50  0000 C CNN
F 2 "" H 9850 2850 50  0001 C CNN
F 3 "" H 9850 2850 50  0001 C CNN
	1    9850 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 2850 9850 3050
$Comp
L power:+5V #PWR?
U 1 1 6181F445
P 9850 4700
F 0 "#PWR?" H 9850 4550 50  0001 C CNN
F 1 "+5V" H 9865 4873 50  0000 C CNN
F 2 "" H 9850 4700 50  0001 C CNN
F 3 "" H 9850 4700 50  0001 C CNN
	1    9850 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 4700 9850 4850
$EndSCHEMATC
