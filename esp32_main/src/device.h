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
    uint8_t id;
    uint8_t subscription;
    Device();
    Device(uint8_t addr, uint8_t id,uint8_t subscription);

    virtual void analyse(){};
};

typedef struct list_device {
    Device* device;
    struct list_device* next_link=NULL;
    struct list_device* prev_link=NULL;
} list_device;

void push(list_device **list_dev,Device* device);

void retreive(list_device **list_dev,uint8_t addr);

Device* access(list_device **list_dev,uint8_t val,const char * attr="addr");


void clear(list_device ** list_dev);



