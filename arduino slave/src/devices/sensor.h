#include "Device.h"

class Sensor : public Device {
public : 
    char*mycommands[4]={strdup("ping"),strdup("change_addr"),strdup("get_info"),strdup("get_value")};
    int current_value =0;

    Sensor();
    void process(volatile uint8_t * buffer, uint8_t len);
    bool check_value (int value);
    bool get_value ();
    SlaveResponse getResponse();
    uint8_t expectedReceiveLength(uint8_t forRegister);
    void doThings();
};