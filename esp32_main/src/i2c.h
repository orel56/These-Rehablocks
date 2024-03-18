#include "Wire.h"
#include "Arduino.h"
#include <config.h>

int send_command(uint8_t addr,const char * command, uint8_t value = 0, uint8_t* value2= nullptr);
void receive_data(uint8_t addr,uint8_t * data_buffer, uint8_t bytesToBeReceived);
