#include "Device.h"
#include <time.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>


class Accelerometer : public Device {
public: 
    int threshold=100;
    float valueX[2]={0.0,0.0};
    float valueY[2]={0.0,0.0};
    float valueZ[2]={0.0,0.0};
    float accel_error[3]={0.08,-0.22,0.02};
    uint8_t sda = 21;
    uint8_t scl =22;
    uint8_t mpu_addr = 0x68;

    bool mouv =0;

    Adafruit_MPU6050 mpu;

    Accelerometer(int id, int threshold);
    Accelerometer();
    void update_param();
    void setup();

    void read_accelero();
    
    void produce_subjects();
    void init_produced_subject();
    void behaviour1();

};