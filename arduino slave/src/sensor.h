#include "Device.h"

class Sensor : public Device {
public : 
    String mycommands[4]={("ping"),("change_addr"),("get_info"),("get_value")};
    int current_value =0;
    bool send_value=0;
    Sensor();
    void process(volatile uint8_t * buffer, uint8_t len);
    virtual bool check_value (int value);
    virtual bool get_value ();
    SlaveResponse getResponse();
    uint8_t expectedReceiveLength(uint8_t forRegister);
    uint8_t* int_to_bytesarray(int value);
    void doThings();
};