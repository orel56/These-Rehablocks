#include <stdint.h>
#include <mat.h>
#include <list>


struct subject {
    uint8_t id;
    int value;
    int prev_value;
    std::list<uint8_t> is_produced_by;
    std::list<uint8_t> are_subscribe;
    std::list<uint8_t> substitute;
    bool to_be_sent;
    bool already_update;
};

typedef struct subject Subject;


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
    bool is_subscribe(uint8_t id);
    virtual void analyse(){};
};




