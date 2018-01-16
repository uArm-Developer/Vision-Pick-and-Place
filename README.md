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

The firmware of uArm should be modified by yourself:

1.Download the original firmware [here](https://github.com/uArm-Developer/SwiftProForArduino)

2.Modify the code following the [guide](http://download.ufactory.cc/docs/en/uArm-Swift-Pro-Quick-Start-Guide-171221.pdf) Page19

3.Compile the code and upload it into the MEGA2560
