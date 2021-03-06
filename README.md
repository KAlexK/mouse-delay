# mouse-delay
Determining the delay from clicking on the mouse button to WM_LBUTTONDOWN (or WM_LBUTTONDBLCLK) message. The description [is here (in Russian)](https://www.ixbt.com/printers/mouse-button-delay-method.html).
[Video with a demonstration of the work (only 10 clicks).](https://youtu.be/kyXcPb9aahc)
 
## Hardware

Connection diagram:
 
![Connection diagram](https://github.com/KAlexK/mouse-delay/blob/main/pics/schematic-01.png)

Main controller is [Seeeduino Xiao](www.seeedstudio.com/Seeeduino-XIAO-Arduino-Microcontroller-SAMD21-Cortex-M0+-p-4426.html):
 
![Seeeduino Xiao](https://github.com/KAlexK/mouse-delay/blob/main/pics/Seeeduino-XIAO-pin-out.jpg)
 
Matching pins of the board and ports of the microcontroller:

![Pinout](https://github.com/KAlexK/mouse-delay/blob/main/pics/pin-out.png)

The mouse button is pressed using the HDD head drive:
 
![HDD head drive](https://github.com/KAlexK/mouse-delay/blob/main/pics/stand-07.jpg)
 
Assembled stand:

![Assembled stand](https://github.com/KAlexK/mouse-delay/blob/main/pics/stand-04.jpg)

More pics: [01](https://github.com/KAlexK/mouse-delay/blob/main/pics/stand-05.jpg), [02](https://github.com/KAlexK/mouse-delay/blob/main/pics/stand-06.jpg), [03](https://github.com/KAlexK/mouse-delay/blob/main/pics/stand-08.jpg), [04](https://github.com/KAlexK/mouse-delay/blob/main/pics/stand-09.jpg).

Driver diagram for HDD drive coil:

![Driver diagram](https://github.com/KAlexK/mouse-delay/blob/main/pics/schematic.png)

See [KiCad project](https://github.com/KAlexK/mouse-delay/tree/main/hardware/driver).

Assembled driver:

![Assembled driver](https://github.com/KAlexK/mouse-delay/blob/main/pics/driver-01.jpg)

![Assembled driver](https://github.com/KAlexK/mouse-delay/blob/main/pics/driver-02.jpg)

To measure the delay (1st delay) from the signal transmission to the connection of the mouse button switch contacts, you need to solder two wires to the button contacts. It is better to put the positive wire through a 1k resistor. Example:

![Button contacts](https://github.com/KAlexK/mouse-delay/blob/main/pics/test-mouse-02.jpg)

## Firmware

[Project for Seeeduino Xiao board.](https://github.com/KAlexK/mouse-delay/tree/main/mc)

The firmware is compiled using MPLAB X IDE v5.50 and XC32 3.01. The initial configuration is created using MPLAB Harmony v3. How to upload the firmware to Seeeduino Xiao board: 1. [Enter Bootloader Mode](https://wiki.seeedstudio.com/Seeeduino-XIAO/#enter-bootloader-mode). 2. Copy file bin.uf2 to Seeeduino Xiao, which appeared as a disk.

## Software

[Visual Studio 2017 Project.](https://github.com/KAlexK/mouse-delay/tree/main/host/mouse-delay)

[wxWidgets 3.1.2](https://github.com/wxWidgets/wxWidgets/releases/tag/v3.1.2/) and [libusb-1.0.lib](http://libusb.info) are used. These dlls are needed for the program to work:
- libusb-1.0.dll
- wxbase312ud_vc141.dll
- wxbase312ud_xml_vc141.dll
- wxmsw312ud_core_vc141.dll
- wxmsw312ud_html_vc141.dll
- wxmsw312ud_xrc_vc141.dll

## How to use

- Install mouse on stand (see the picture above).
- Connect all together (see the connection diagram), including DC 12V source to coil driver and optional wires from mouse button.
- Start mouse-delay.exe. Press "Connect" button. Enter "Number of tests" - 100 for example. Enter "Delay between tests, ms", 500 is good. Check "1st delay" if you want to measure 1st delay and wires from mouse button are connected. Place mouse cursor on yellow area. Everything is ready to run the test!
- To start measurements press "Start!" button using the keyboard.

After the end of the test, the program window will look like this:

![Program window](https://github.com/KAlexK/mouse-delay/blob/main/pics/soft.png)

You can copy the results from the window marked as "Delays:".
