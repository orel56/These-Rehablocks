#include "Device.h"
#include <time.h>

class Accelerometer : public Device {
public: 
    int elapsed_time =0;
    int threshold=100;

    Accelerometer(int id);
    Accelerometer();
    void update_param();
    void update_subject();

    void behaviour1();
    void behaviour2();

    bool set_value(int value);
    bool is_setable(int value);
};