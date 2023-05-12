#include "Device.h"

class Potentiometer : public Device {

public:
    int adc_pin=0;
    int threshold=0;
    Potentiometer(int adc_pin,int threshold);
    Potentiometer();
    Potentiometer(int adc_pin, int threshold, int id);
    bool get_value();
    bool check_value(int value);
};