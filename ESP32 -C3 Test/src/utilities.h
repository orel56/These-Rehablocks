#include "Wire.h"
#include "Arduino.h"
#include <config.h>

struct devices {
    uint8_t addr;
    int value;
    int id;
    int type;
    int subscription;
};

typedef struct devices device;


int send_command(uint8_t addr,const char * command, uint8_t value = 0);
void receive_data(uint8_t addr,uint8_t * data_buffer, uint8_t bytesToBeReceived);

uint8_t * intToBytesArray(int value);
int bytesArraytoInt(volatile uint8_t* data,uint8_t len,uint8_t begin_val);
