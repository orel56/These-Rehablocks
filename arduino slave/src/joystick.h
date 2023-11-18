#include "Device.h"

class joystick : public Device {

public:
    int pinx=0;
    int piny=0;
    int threshold=0;

    int valueX[2]={0,0};
    int valueY[2]={0,0};
    int drift[2]={0,0};

    bool firsttime=1;
    joystick();
    joystick(int pinx,int piny, int threshold, int id);
    void update_param();

    uint8_t get_direction();

    void produce_subjects();
    void init_produced_subject();
    void behaviour1();
};