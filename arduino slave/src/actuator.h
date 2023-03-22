#include "Device.h"

class Actuator : public Device {
public : 
    String mycommands[4]={("ping"),("change_addr"),("get_info"),("set_value")};
    int current_value =0;
    bool is_set=true;

    Actuator();
    void process(volatile uint8_t * buffer, uint8_t len);
    virtual bool is_setable (int value);
    virtual bool set_value (int value);
    SlaveResponse getResponse();
    int bytesArraytoInt(volatile uint8_t* data, uint8_t len);

};