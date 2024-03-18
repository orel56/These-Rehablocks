#include "Wire.h"
#include "Arduino.h"
#include <config.h>
#if (ENABLE_BLE)
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#endif

#include <string.h>


struct subject {
    uint8_t id; 
    int value;
};

typedef struct subject Subject;

struct devices {
    uint8_t addr;
    int status;
    int id;
    int type;
    int subscription;
    Subject *subjects[20];
    int sub_number = 0;
};

typedef struct devices device;


int send_command(uint8_t addr,const char * command, uint8_t value = 0, uint8_t* value2= nullptr);
void receive_data(uint8_t addr,uint8_t * data_buffer, uint8_t bytesToBeReceived);

void intToBytesArray(int value, uint8_t bytes[4]);
int bytesArraytoInt(volatile uint8_t* data,uint8_t len,uint8_t begin_val);

int init_ble();
void check_ble(device * nodes[10], uint8_t n);
std::string data_to_send(device * nodes[10], uint8_t n);