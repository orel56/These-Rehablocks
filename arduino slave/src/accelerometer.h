#include "Device.h"
#include "BitBang_I2C.h"
class Accelerometer : public Device {
public: 
    int threshold=100;
    float valueX[2]={0.0,0.0};
    float valueY[2]={0.0,0.0};
    float valueZ[2]={0.0,0.0};
    float accel_error[2]={0.08,-0.22};
    uint8_t sda = A5;
    uint8_t scl = A4;
    uint8_t mpu_addr = 0x68;

    bool mouv =0;

    Accelerometer(int id, int threshold);
    Accelerometer();
    void update_param();
    void setup();

    void calculate_IMU_error();

    void read_accelero();
    void produce_subjects();
    void init_produced_subject();
    void behaviour1();

};