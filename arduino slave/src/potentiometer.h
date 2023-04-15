#include "sensor.h"

class Potentiometer : public Sensor {

public:
    int adc_pin=0;
    int threshold=0;
    Potentiometer(int adc_pin,int threshold);
    Potentiometer();
    Potentiometer(int adc_pin, int threshold, int id);
    Potentiometer(int adc_pin, int threshold,int id, int linkId);
    bool get_value();
    bool check_value(int value);
    
};