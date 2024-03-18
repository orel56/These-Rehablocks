#include "Device.h"

class Potentiometer : public Device {

public:
    int adc_pin=0;
    int threshold=0;
    Potentiometer();
    Potentiometer(int adc_pin, int threshold, int id);
    void update_param();

    void produce_subjects();
    void init_produced_subject();
    void behaviour1();
};