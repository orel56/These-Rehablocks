#include "actuator.h"

class Led : public Actuator {
public: 
    int pin=0;
    float frequency=0.0;
    Led(int pin);
    Led(int pin,int id);
    Led(int pin,int id, int linkId);

    Led();
    bool set_value(int value);
    bool is_setable(int value);
};