#include "sensor.h"

class Potentiometer : public Sensor {

public:
    int adc_pin=0;
    int threshold=0;
    Potentiometer(int adc_pin,int threshold);
    bool get_value();
    bool check_value(int value);
    
};