#include "Device.h"
#include <time.h>

class Led : public Device {
public: 
    int pin=0;
    float frequency=1;
    float period=1;
    uint8_t max_time=15;
    int elapsed_time =0;
    int timer=0;
    double t=0.0;
    
    Led(int pin);
    Led(int pin,int id);
    Led();
    void update_param();
    void init_produced_subject();
    void produce_subjects();

    void behaviour1();
    void behaviour2();
};