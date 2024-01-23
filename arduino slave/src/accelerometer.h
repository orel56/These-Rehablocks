#include "Device.h"
#include <time.h>

class Accelerometer : public Device {
public: 
    int threshold=100;
    float valueX[2]={0.0,0.0};
    float valueY[2]={0.0,0.0};
    float valueZ[2]={0.0,0.0};
    float drift[2]={0.0,0.0};
    uint8_t sda = 21;
    uint8_t scl =22;
    uint8_t mpu_addr = 0x68;

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