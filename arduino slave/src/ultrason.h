#include "Device.h"

class ultrason : public Device {

public:
    int pintrig=0;
    int pinecho=0;
    int threshold=0;


    int distance[2]={0,0};

    bool firsttime=1;
    ultrason();
    ultrason(int pintrig,int pinecho, int threshold, int id);
    void update_param();

    uint8_t get_distance();

    void produce_subjects();
    void init_produced_subject();
    void behaviour1();
};