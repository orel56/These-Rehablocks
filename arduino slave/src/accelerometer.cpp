#include <accelerometer.h>

BBI2C bbi2c;

#define BITBANG false

uint8_t read_data[8];
uint8_t data[5];

Accelerometer::Accelerometer(int id, int threshold){

  this->id=id;
  this->threshold=threshold;
  this->init_produced_subject();

};

Accelerometer::Accelerometer(){};

void Accelerometer::init_produced_subject(){
    this->produced_subject_nbr=2;
    this->producedSubjects[0]= new Subject(0);
    //this->producedSubjects[1]= new Subject(2);
    //this->producedSubjects[2]= new Subject(3);
    this->producedSubjects[1]= new Subject(1);
}
void Accelerometer::setup(){
  int error=0;
  memset(&bbi2c, 0, sizeof(bbi2c));
  bbi2c.bWire = BITBANG; // use bit bang, not wire library
  bbi2c.iSDA = this->sda;
  bbi2c.iSCL = this->scl;
  I2CInit(&bbi2c, 100000L);
  this->i2c_detect();
  delay(25);


  data[0] = 0x6B;
  data[1] = 0x00;
  error = I2CWrite(&bbi2c, this->mpu_addr, data, 2);
  Serial.println(error);
  data[0] = 0x1C;
  data[1] = 0x10;
  error = I2CWrite(&bbi2c, this->mpu_addr, data, 2);
  Serial.println(error);

  data[0] = 0x1B;
  data[1] = 0x10;
  error = I2CWrite(&bbi2c, this->mpu_addr, data, 2);
  Serial.println(error);

  // Call this function if you need to get the IMU error values for your module
  //calculate_IMU_error();
  delay(5); // allow devices to power up

}

void Accelerometer::behaviour1(){
  float accel_x, accel_y, accel_z, rot_x, rot_y = 0.0;
  int error=0;
  mouv=0;
  data[0] = 0x3B;
  error = I2CWrite(&bbi2c, this->mpu_addr, data, 1);
  Serial.println(error);

  error = I2CRead(&bbi2c, this->mpu_addr, read_data, 6);
  Serial.println(error);

  // For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  accel_x = (read_data[0] << 8 | read_data[1]) / 16384.0; // X-axis value
  accel_y = (read_data[2] << 8 | read_data[3]) / 16384.0; // Y-axis value
  accel_z = (read_data[4] << 8 | read_data[5]) / 16384.0; // Z-axis value
  // Calculating Roll and Pitch from the accelerometer data
  rot_x = (atan((accel_z) / sqrt(pow((accel_y), 2) + pow((accel_x), 2))) * 180 / 3.14) - this->accel_error[0];      // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
  rot_y = (atan((accel_y) / sqrt(pow((accel_z), 2) + pow((accel_x), 2))) * 180 / 3.14) - this->accel_error[1]; // AccErrorY ~(-1.58)
  // === Read gyroscope data === //

    Serial.print("accel X : ");
    Serial.println(rot_x);
    Serial.print("accel y : ");
    Serial.println(rot_y);


  if (fabs(rot_x - this->valueX[0])>this->threshold){
    this->valueX[1]=this->valueX[0];
    this->valueX[0] = rot_x;
    mouv=1;
  }

  if (fabs(rot_y - this->valueY[0])>this->threshold){
    this->valueY[1]=this->valueY[0];
    this->valueY[0] = rot_y;
    mouv=1;
  }
}


void Accelerometer::produce_subjects()
{
    for (int id = 0; id < this->produced_subject_nbr; id++)
    {
        if ((this->producedSubjects[id])->id == 2)
        {
            (this->producedSubjects[id])->old_value = (this->producedSubjects[id])->value;
                floatToInt converter;
                converter.nbf = valueZ[0];
                uint32_t y = converter.nbe;
            (this->producedSubjects[id])->value = y;

        }
        else if ((this->producedSubjects[id])->id == 3){
            (this->producedSubjects[id])->old_value = (this->producedSubjects[id])->value;
                floatToInt converter;
                converter.nbf = valueY[0];
                uint32_t y = converter.nbe;
            (this->producedSubjects[id])->value = y;
        }
        else if ((this->producedSubjects[id])->id == 1){
            (this->producedSubjects[id])->old_value = (this->producedSubjects[id])->value;
            (this->producedSubjects[id])->value = mouv;
            Serial.print("mouvement value is : ");
            Serial.println((this->producedSubjects[id])->value);
        }
        else
        {
            (this->producedSubjects[id])->old_value = 1;
            (this->producedSubjects[id])->value = 1;
        }
    }
}

void Accelerometer::calculate_IMU_error()
{
  // We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
  // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
  // Read accelerometer values 200 times
float AccX, AccY, AccZ;
float AccErrorX, AccErrorY=0.0;
int c = 0;
  int cpt=0;
  while (c < 200)
  {
  data[0] = 0x3B;
    I2CRead(&bbi2c, this->mpu_addr, read_data, 6);

    // For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
    AccX = (read_data[0] << 8 | read_data[1]) / 16384.0; // X-axis value
    AccY = (read_data[2] << 8 | read_data[3]) / 16384.0; // Y-axis value
    AccZ = (read_data[4] << 8 | read_data[5]) / 16384.0; // Z-axis value

    if (sqrt(pow((AccY), 2) + pow((AccX), 2))!=0.0 && sqrt(pow((AccZ), 2) + pow((AccX), 2))!=0.0) {
    // Sum all readings
    AccErrorX = AccErrorX + (atan((AccZ) / sqrt(pow((AccY), 2) + pow((AccX), 2))) * 180 / 3.14);
    AccErrorY = AccErrorY + (atan((AccY) / sqrt(pow((AccZ), 2) + pow((AccX), 2))) * 180 / 3.14);
    cpt++;
    }
    c++;


  }
  // Divide the sum by 200 to get the error value
  this->accel_error[0] = AccErrorX / cpt;
  this->accel_error[1] = AccErrorY / cpt;

  // Print the error values on the Serial Monitor
}

void Accelerometer::update_param(){
  
}

void Accelerometer::i2c_detect()
{
  uint8_t map[16];
  char szTemp[32];
  uint8_t i;
  int iDevice, iCount;
  uint32_t u32Caps;

  Serial.println("Starting I2C Scan");
  I2CScan(&bbi2c, map); // get bitmap of connected I2C devices
  if (map[0] == 0xfe)   // something is wrong with the I2C bus
  {
    Serial.println("I2C pins are not correct or the bus is being pulled low by a bad device; unable to run scan");
  }
  else
  {
    iCount = 0;
    for (i = 0; i < 128; i++) // skip address 0 (general call address) since more than 1 device can respond
    {
      if (map[i >> 3] & (1 << (i & 7))) // device found
      {
        iCount++;
        Serial.print("Device found at 0x");
        Serial.print(i, HEX);
        iDevice = I2CDiscoverDevice(&bbi2c, i, &u32Caps);
        Serial.print(", type = ");
        I2CGetDeviceName(iDevice, szTemp);
        Serial.print(szTemp); // show the device name as a string
        Serial.print(", capability bits = 0x");
        Serial.println(u32Caps, HEX);
      }
    } // for i
    Serial.print(iCount, DEC);
    Serial.println(" device(s) found");
  }
}