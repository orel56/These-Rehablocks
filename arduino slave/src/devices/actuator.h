#include "Device.h"

class Actuator : public Device {
public : 
    char*mycommands[4]={strdup("ping"),strdup("change_addr"),strdup("get_info"),strdup("set_value")};
    int current_value =0;


    Actuator();
    void process(volatile uint8_t * buffer, uint8_t len);
    bool is_setable ();
    bool set_value (int value);
    SlaveResponse getResponse();
    uint8_t expectedReceiveLength(uint8_t forRegister);
    void doThings();

};