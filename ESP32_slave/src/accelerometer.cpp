#include <accelerometer.h>


Accelerometer::Accelerometer(int id, int threshold){

  this->id=id;
  this->threshold=threshold;
  this->init_produced_subject();

};

Accelerometer::Accelerometer(){};

void Accelerometer::init_produced_subject(){
    this->produced_subject_nbr=5;
    this->producedSubjects[0]= new Subject(0b00000001);
    this->producedSubjects[1]= new Subject(0b10000000);
    this->producedSubjects[2]= new Subject(0b01000000);
    this->producedSubjects[3]= new Subject(0b00100000);
    this->producedSubjects[4]= new Subject(0b00010000);
}
void Accelerometer::setup(){
  if (!mpu.begin(MPU6050_I2CADDR_DEFAULT,&Wire))
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

void Accelerometer::behaviour1(){
  float accel_x, accel_y, accel_z, rot_x, rot_y = 0.0;
  sensors_event_t a, g, temp;

  mpu.getEvent(&a, &g, &temp);

  accel_x = a.acceleration.x - accel_error[0];
  accel_y = a.acceleration.y - accel_error[1];
  accel_z = a.acceleration.z - accel_error[2];

  rot_x = atan((accel_z) / sqrt(pow((accel_y), 2) + pow((accel_x), 2))) * 180 / 3.14;
  rot_y = atan((accel_y) / sqrt(pow((accel_z), 2) + pow((accel_x), 2))) * 180 / 3.14;

  if (fabs(rot_x - this->valueX[0])>this->threshold){
    this->valueX[1]=this->valueX[0];
    this->valueX[0] = rot_x;
    mouv=1;
  }
  else {
    mouv=0;
  }
  if (fabs(rot_y - this->valueY[0])>this->threshold){
    this->valueY[1]=this->valueY[0];
    this->valueY[0] = rot_y;
  }
  else {
    mouv=0;
  }
}


void Accelerometer::produce_subjects()
{
    for (int id = 0; id < this->produced_subject_nbr; id++)
    {
        if ((this->producedSubjects[id])->id == 0b00010000)
        {
            // sujet mouvement qui est catch par la led 
            (this->producedSubjects[id])->old_value = (this->producedSubjects[id])->value;
                floatToInt converter;
                converter.nbf = valueZ[0];
                uint32_t y = converter.nbe;
            (this->producedSubjects[id])->value = y;

        }
        else if ((this->producedSubjects[id])->id == 0b00100000){
            (this->producedSubjects[id])->old_value = (this->producedSubjects[id])->value;
                floatToInt converter;
                converter.nbf = valueY[0];
                uint32_t y = converter.nbe;
            (this->producedSubjects[id])->value = y;
        }
        else if ((this->producedSubjects[id])->id == 0b01000000){
            (this->producedSubjects[id])->old_value = (this->producedSubjects[id])->value;
                floatToInt converter;
                converter.nbf = valueX[0];
                uint32_t y = converter.nbe;
            (this->producedSubjects[id])->value = y;
        }
        else if ((this->producedSubjects[id])->id == 0b10000000){
            (this->producedSubjects[id])->old_value = (this->producedSubjects[id])->value;
            (this->producedSubjects[id])->value = mouv;
        }
        else
        {
            (this->producedSubjects[id])->old_value = 1;
            (this->producedSubjects[id])->value = 1;
        }
    }
}

void Accelerometer::update_param(){
  
}