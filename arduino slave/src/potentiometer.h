#include "Device.h"

class Potentiometer : public Device {

public:
    int adc_pin=0;
    int threshold=0;
    Potentiometer();
    Potentiometer(int adc_pin, int threshold, int id);
    void update_param();
    void update_subject();
    void get_status();

    void behaviour1();
};