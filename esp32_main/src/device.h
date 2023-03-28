#include <stdint.h>

class Device {
public :
    uint8_t addr;
    uint8_t type;
    int current_value;
    int previous_value;
    bool quarantine; 
    bool to_be =0;
    Device();
    Device(uint8_t addr, uint8_t type);
};

class Sensor : public Device {
public : 
    bool to_be_send;

    Sensor();
    Sensor(uint8_t addr, uint8_t type);

};

class Actuator : public Device {
public : 
    bool to_be_set;
    Actuator();
    Actuator(uint8_t addr, uint8_t type);
};