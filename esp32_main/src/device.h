#include <stdint.h>
#include <mat.h>
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

    virtual void analyse(){};
};

class Sensor : public Device {
public : 
    int threshold =0;
    Sensor();
    Sensor(uint8_t addr, uint8_t type,int threshold);
    virtual void analyse();

    ~Sensor(){};
};

class Actuator : public Device {
public : 
    uint8_t linked;
    Actuator();
    Actuator(uint8_t addr, uint8_t type,uint8_t linked);
    virtual void analyse();

    ~Actuator(){};
};