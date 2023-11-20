#include "Wire.h"
#include "Arduino.h"
#include <config.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <string.h>

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
std::string dataToSend = "";

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


struct devices {
    uint8_t addr;
    int status;
    int id;
    int type;
    int subscription;
    subject *my_subjects[20];
};

struct subject {
    uint8_t id; 
    int value;
};

typedef struct subject Subject;

typedef struct devices device;

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  }

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};


int send_command(uint8_t addr,const char * command, uint8_t value = 0, uint8_t* value2= nullptr);
void receive_data(uint8_t addr,uint8_t * data_buffer, uint8_t bytesToBeReceived);

void intToBytesArray(int value, uint8_t bytes[4]);
int bytesArraytoInt(volatile uint8_t* data,uint8_t len,uint8_t begin_val);

int init_ble();
std::string data_to_send(device * nodes[10], uint8_t n);