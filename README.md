# Vision-Pick-and-Place
Vision system with uArm Swift Pro and OpenMV

[Video](https://www.youtube.com/watch?v=BEpHAaJll_g)

## Hardware  Preparation

[uArm Swift Pro](www.ufactory.cc) * 1

[Arduino MEGA2560](www.arduino.cc) * 1

[OpenMV](www.openmv.io) * 1

Several other parts

## Software Preparation

Vision.ino is an arduino file for the MEGA2560

color_tracking_test.py is the code for OpenMV based on OpenMV IDE

The uArm firmware should be uploaded by yourself:

1.Download the XLoader [here](http://xloader.russemotto.com/)

2.Set the parameter below

    Hex file: uArm Firmware.hex

    Device: Mega(ATMEGA2560)

    COM port: depending on your system

    Baud rate: 115200

3.Click Upload to finish the flashing
