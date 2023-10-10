# Arduino_MPU6050_controller
Arduino code to read MPU6050 accelerometer/gyroscope sensor using I2C

This repository provides an Arduino code to communicate with MPU6050 accelerometer/gyroscope sensor module and obtain readings. 
The source code uses Wire.h library functions to transmit and receive data using I2C bus. 
https://www.arduino.cc/reference/en/language/functions/communication/wire/

The default pins for I2C data and clock are Analog4 and Analog5 pins respectively. The module also needs 5V and GND connections

![image](https://github.com/melihaltun/Arduino_MPU6050_controller/assets/40482921/72427624-37e4-4e6a-b9e9-cccf196f2f52)

After an initial calibration the code keeps reading registers that hold accelerometer and gyroscope measurements.
The read values are scaled, compensated with calibration values and used to calculate roll, pitch and yaw angles. 
The measured a_x, a_y, a_z (in g) and, roll, pitch, yaw angles (in radians) are repored with the serial monitor. 
