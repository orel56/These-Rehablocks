#include "Device.h"
#include <time.h>

class Accelerometer : public Device {
public: 
    int threshold=100;
    float rot_x, rot_y, rot_z;

    Accelerometer(int id);
    Accelerometer();
    void update_param();
    void update_subject();
    void get_status();
    void setup();

    void read_accelero();
    void behaviour1();
    void behaviour2();

};