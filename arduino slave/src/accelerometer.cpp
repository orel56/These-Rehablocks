#include <accelerometer.h>
/* #include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

static Adafruit_MPU6050 mpu;

static float rotation_x_error = -0.05;
static float rotation_y_error = 0.02;
static float rotation_z_error = 0.03;
static float accel_x_error = 0.08;
static float accel_y_error = -0.22;
static float accel_z_error = 0.01;



Accelerometer::Accelerometer(int id){

  this->id = id;
#ifdef ACCELEROMETER_THRESHOLD
    this->threshold= ACCELEROMETER_THRESHOLD
#endif
};

void Accelerometer::setup(){

  if (!mpu.begin())
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
    {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

}

Accelerometer::Accelerometer(){
};

void Accelerometer::read_accelero(){
  float accel_x, accel_y, accel_z = 0.0;
  
  sensors_event_t a, g, temp;

  mpu.getEvent(&a, &g, &temp);

  accel_x = a.acceleration.x - accel_x_error;
  accel_y = a.acceleration.y - accel_y_error;
  accel_z = a.acceleration.z - accel_z_error;

  this->rot_x = atan((accel_z) / sqrt(pow((accel_y), 2) + pow((accel_x), 2))) * 180 / 3.14;
  this->rot_y = atan((accel_y) / sqrt(pow((accel_z), 2) + pow((accel_x), 2))) * 180 / 3.14;
}

void Accelerometer::behaviour1()
{
   this->read_accelero();
   static float rot_x_old, rot_y_old, rot_z_old;

    if ((fabs(rot_x_old - this->rot_x) > this->threshold) || (fabs(rot_y_old - this->rot_y) > this->threshold)) {
        rot_x_old = this->rot_x;
        rot_y_old = this->rot_y;
    } 
}
void Accelerometer::update_param(){
     if(this->pendingCommand[1]==1){
        this->threshold=bytesArraytoInt(pendingCommand,5,2);
    }
}

void Accelerometer::update_subject(){
   
}

void Accelerometer::get_status(){
    SlaveResponse resp;
    resp.buffer[0]=this->acknowledge;
    resp.buffer[1]=0;
    uint8_t* value = floatToBytesArray(this->rot_x);
    resp.buffer[2] = value[0];
    resp.buffer[3] = value[1];
    resp.buffer[4] = value[2];
    resp.buffer[5] = value[3];
    value = floatToBytesArray(this->rot_y);
    resp.buffer[6] = value[0];
    resp.buffer[7] = value[1];
    resp.buffer[8] = value[2];
    resp.buffer[9] = value[3];
    resp.size=10;
    this->status=resp;

} */