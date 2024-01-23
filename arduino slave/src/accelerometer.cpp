#include <accelerometer.h>


Accelerometer::Accelerometer(int id, int threshold){

  this->id=id;
  this->threshold=threshold;

};

Accelerometer::Accelerometer(){};

void Accelerometer::init_produced_subject(){
    this->produced_subject_nbr=5;
    this->producedSubjects[0]= new Subject(0b00000001);
    this->producedSubjects[1]= new Subject(0b10000000);
    this->producedSubjects[1]= new Subject(0b01000000);
    this->producedSubjects[1]= new Subject(0b00100000);
    this->producedSubjects[1]= new Subject(0b00010000);
}
void Accelerometer::setup(){
  Wire.begin(); //to change regarding the type of mcu

  Wire.beginTransmission(this->mpu_addr);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission

}