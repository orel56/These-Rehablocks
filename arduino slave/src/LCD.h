#include <Device.h>

#include "Device.h"

class LCD : public Device {
public: 
    int pin=0;
    float frequency=0.5;
    float period=2;
    uint8_t max_time=20;
    unsigned long elapsed_time =0;
    float timer=0;
    double t=0.0;
    
    LCD(int pin,int id);
    LCD();
    void update_param();
    void init_produced_subject();
    void produce_subjects();

    void behaviour1();
    void behaviour2();
};