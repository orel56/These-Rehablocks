#include "Device.h"
#include <time.h>

class Led : public Device {
public: 
    int pin=0;
    float frequency=1;
    float period=1;
    uint8_t max_time=15;

    unsigned long elapsed_time =0;
    unsigned long timer=0;
    double t=0.0;
    bool mouv =0;

    Led(int pin);
    Led(int pin,int id);
    Led();
    void update_param();
    void update_subject();

    void behaviour1();
    void behaviour2();

    bool set_value(int value);
    bool is_setable(int value);
};