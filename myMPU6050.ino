/* Arduino MPU6050 Accel controller code */

#include <Wire.h>
const int MPU6050_addr = 0x68; // MPU6050 I2C address: 0x68 if AD0 pin is low or NC, 0x69 if AD0 is pulled high
const float gyroScaleFactor = 131.0;
const float ADC_range = 65536.0;
float accl_x, accl_y, accl_z, gyro_x, gyro_y, gyro_z;
float accAngle_x, accAngle_y, gyroAngle_x, gyroAngle_y, gyroAngle_z;
float accOffset_x = 0, accOffset_y = 0, gyroOffset_x = 0, gyroOffset_y = 0, gyroOffset_z = 0;
float roll, pitch, yaw;
float elapsedTime, currentTime, previousTime;

void setup() {
  Serial.begin(19200);                // serial monitor baud rate
  Wire.begin();                      // initialize I2C
  Wire.beginTransmission(MPU6050_addr);     // start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // point to the register 6B  // PWR_MGMT1  R/W    b7:reset, 6:sleep, 5:cyclem 4/ 3:Temp_dis, b2,1,0: clk_sel[2:0] 
  Wire.write(0x00);                  // write 00 to register 6B to reset the device
  Wire.endTransmission(true);        // terminate transmission, release the I2C bus
  calibrate_IMU();                   // call initial calibration
  currentTime = millis();   // get initial current time reading in milli secs.
}

void loop() {
  readAccelData();  // obtain accl_x, accl_y and accl_z;
  // calculate accelerometer roll and pitch angles
  accAngle_x = (atan(accl_y / sqrt(pow(accl_x, 2) + pow(accl_z, 2))) * 180 / PI) - accOffset_x; // apply offsets from initial calibration function
  accAngle_y = (atan(-1 * accl_x / sqrt(pow(accl_y, 2) + pow(accl_z, 2))) * 180 / PI) - accOffset_y; 
  
  readGyroData();  // obtain gyro_x, gyro_y and gyro_z;
  // apply calibration offsets
  gyro_x = gyro_x - gyroOffset_x; 
  gyro_y = gyro_y - gyroOffset_y;
  gyro_z = gyro_z - gyroOffset_z;
  
  previousTime = currentTime;        // store prev. time
  currentTime = millis();            // current time in milliseconds
  elapsedTime = (currentTime - previousTime) / 1000; // get elapsed time in seconds
  
  // use elapsed time to convert raw rad/s readings to roll, pitch and yaw angles 
  gyroAngle_x += gyro_x * elapsedTime; 
  gyroAngle_y += gyro_y * elapsedTime;
  yaw += gyro_z * elapsedTime;
  //mix in accelerometer angle data to minimize drift
  roll = 0.96 * gyroAngle_x + 0.04 * accAngle_x;
  pitch = 0.96 * gyroAngle_y + 0.04 * accAngle_y;
  
  // Print outputs
  Serial.print("ax: ");
  Serial.print(accl_x);
  Serial.print(" ay: ");
  Serial.print(accl_y);
  Serial.print(" az: ");
  Serial.print(accl_z);
  Serial.print(" - Roll: ");
  Serial.print(roll);
  Serial.print(" Pitch: ");
  Serial.print(pitch);
  Serial.print(" Yaw: ");
  Serial.println(yaw);
}

void calibrate_IMU() {
  // Function to calculate accel. and gyro. offsets. The sensor must be flat and stationary when this subroutine runs initially.
  const int avgLen = 200;  
  // Read accelerometer values avgLen (averaging length) times  
  for(int i = 0; i < avgLen; i++) {
    readAccelData();    // obtain accl_x, accl_y and accl_z;
    // accumulate readings 
    accOffset_x += atan((accl_y) / sqrt(pow((accl_x), 2) + pow((accl_z), 2))) * 180 / PI;
    accOffset_y += atan(-1 * (accl_x) / sqrt(pow((accl_y), 2) + pow((accl_z), 2))) * 180 / PI;
  }
  //calculate the averaged accelerator offsets
  accOffset_x = accOffset_x / avgLen;
  accOffset_y = accOffset_y / avgLen;
  
  // Read gyro values avgLen (averaging length) times
  for(int i = 0; i < avgLen; i++) {
    readGyroData();  // obtain gyro_x, gyro_y and gyro_z;
    // accumulate readings
    gyroOffset_x += gyro_x;
    gyroOffset_y += gyro_y;
    gyroOffset_z += gyro_z;
  }
  //calculate the averaged gyro offsets
  gyroOffset_x = gyroOffset_x / avgLen;
  gyroOffset_y = gyroOffset_y / avgLen;
  gyroOffset_z = gyroOffset_z / avgLen;
  // Print the offset values
  Serial.print("accOffset_x: ");
  Serial.println(accOffset_x);
  Serial.print("accOffset_y: ");
  Serial.println(accOffset_y);
  Serial.print("gyroOffset_x: ");
  Serial.println(gyroOffset_x);
  Serial.print("gyroOffset_y: ");
  Serial.println(gyroOffset_y);
  Serial.print("gyroOffset_z: ");
  Serial.println(gyroOffset_z);
}

void readAccelData() {
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);  // accel_xout_h: accel_xout[15:8]   => point to register 3B
  Wire.endTransmission(false);   // stop current transmission, keep connection active
  Wire.requestFrom(MPU6050_addr, 6, true); // read next the 6 register values: accel_xout_h, accel_xout_l, accel_yout_h, accel_yout_l, accel_zout_h, accel_zout_l
  // Default sensitivity range is +/-2g. With 2^16 bit ADC, 1g corresponds to 2^14 ADC counts
  accl_x = (Wire.read() << 8 | Wire.read()) / (ADC_range / 4);
  accl_y = (Wire.read() << 8 | Wire.read()) / (ADC_range / 4);
  accl_z = (Wire.read() << 8 | Wire.read()) / (ADC_range / 4);
}

void readGyroData() {
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x43);  // gyro_xout_h: gyro_xout[15:8]   => point to register 43
  Wire.endTransmission(false);   // stop current transmission, keep connection active
  Wire.requestFrom(MPU6050_addr, 6, true);  // read the next 6 register values: gyro_xout_h, gyro_xout_l, gyro_yout_h, gyro_yout_l, gyro_zout_h, gyro_zout_l
  // Gyroscope sensitivity scale factor is 131. This provides a 250deg/s range.
  gyro_x = (Wire.read() << 8 | Wire.read()) / gyroScaleFactor; 
  gyro_y = (Wire.read() << 8 | Wire.read()) / gyroScaleFactor;
  gyro_z = (Wire.read() << 8 | Wire.read()) / gyroScaleFactor;
}
