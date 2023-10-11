# Arduino_MPU6050_controller
Arduino code to read MPU6050 accelerometer/gyroscope sensor data using I2C

![image](https://github.com/melihaltun/Arduino_MPU6050_controller/assets/40482921/98147bce-a7b5-4a08-bed1-5d8824a82f93)

This repository provides an Arduino code to communicate with MPU6050 accelerometer/gyroscope sensor module and obtain readings. 
The source code uses Wire.h library functions to transmit and receive data using I2C bus. 
https://www.arduino.cc/reference/en/language/functions/communication/wire/

The default pins for I2C data and clock are Analog4 and Analog5 pins respectively. The module also needs 5V and GND connections

![image](https://github.com/melihaltun/Arduino_MPU6050_controller/assets/40482921/9a943675-b773-4831-911f-b620ba3fc1ab)

After an initial calibration the code keeps reading registers that hold accelerometer and gyroscope measurements.
The read values are scaled, compensated with calibration values and used to calculate roll, pitch and yaw angles. 
The measured a_x, a_y, a_z (in g) and, roll, pitch, yaw angles (in radians) are repored with the serial monitor. 

MPU6050 datasheet:
https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf

Register map:
https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
