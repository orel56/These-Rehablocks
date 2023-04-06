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

typedef struct list_device {
    Device* device;
    struct list_device* next_link=NULL;
    struct list_device* prev_link=NULL;
} list_device;

void push(list_device **list_dev,Device* device);

void retreive(list_device **list_dev,uint8_t addr);

Device* access(list_device **list_dev,uint8_t addr);


void clear(list_device ** list_dev);



