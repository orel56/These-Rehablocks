#include <stdint.h>
#include <mat.h>


class Device {
public :
    uint8_t addr;
    uint8_t status;
    int current_value;
    int previous_value;

    float x,y,prev_x,prev_y;
   // bool quarantine; 

    int id;
    uint8_t type;
    uint8_t sub_type;

    uint8_t familly;
    uint8_t sub_fam;


    uint8_t subscription;

    uint8_t current_behaviour;
    Device();
    Device(uint8_t addr, int id,uint8_t subscription, uint8_t behaviour);

    void extract_id_info();

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



