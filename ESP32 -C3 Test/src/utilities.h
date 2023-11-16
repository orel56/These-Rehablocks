#include "Wire.h"
#include "Arduino.h"
#include <config.h>

struct devices {
    uint8_t addr;
    int status;
    int id;
    int type;
    int subscription;
};

struct subject {
    uint8_t id; 
    int value;
};

typedef struct subject Subject;

typedef struct devices device;


int send_command(uint8_t addr,const char * command, uint8_t value = 0, uint8_t* value2= nullptr);
void receive_data(uint8_t addr,uint8_t * data_buffer, uint8_t bytesToBeReceived);

void intToBytesArray(int value, uint8_t bytes[4]);
int bytesArraytoInt(volatile uint8_t* data,uint8_t len,uint8_t begin_val);
