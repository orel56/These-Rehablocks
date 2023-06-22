#include <accelerometer.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;


Accelerometer::Accelerometer(int id){

  mpu.begin();
  //setupt motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  this->id = id;
#ifdef ACCELERO_THRESHOLD
    this->threshold= ACCELERO_THRESHOLD
#endif
};

Accelerometer::Accelerometer(){
};

void Accelerometer::behaviour1(){
    
}