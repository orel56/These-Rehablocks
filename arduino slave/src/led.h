#include "actuator.h"

class Led : public Actuator {
public: 
    int pin=0;
    float frequency=0.0;
    Led(int pin);
    bool set_value(int value);
    bool is_setable(int value);
};