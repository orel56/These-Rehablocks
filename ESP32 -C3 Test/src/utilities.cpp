#include <utilities.h>

volatile uint8_t receivedBytes[RECEIVED_COMMAND_MAX_BYTES];

uint8_t data2[50];

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
std::string dataToSend = "";

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"



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


int send_command(uint8_t addr, const char *command, uint8_t value, uint8_t* value2)
{
    uint8_t bytes;
    int out=1;
    if (command == "ping")
    {
        data2[0] = 0x00;
        bytes = 1;
        Serial.println("setup ping");
    }
    else if (command == "change_addr")
    {
        data2[0] = 0x01;
        data2[1] = value;
        bytes = 2;
    }
    else if (command == "get_info")
    {
        data2[0] = 0x02;
        bytes = 1;
    }
    else if ( command == "get_status"){
        data2[0] = 0x03;
        bytes=1;
    }
    else if (command=="publish_subject")
    {
        Serial.println("publishing subject wirtin data");
        data2[0]=0x04;
        data2[1]=0x00;
        data2[2]=value; 
        data2[3]=value2[0];
        data2[4]=value2[1];
        data2[5]=value2[2];
        data2[6]=value2[3];
        bytes=7;
    }
    else
    {
        Serial.println("Don't understand the command");
    }

 Wire.beginTransmission(addr);
    for (int i = 0; i < bytes; i++)
    {
        Wire.write(data2[i]);
    }
    out = Wire.endTransmission();
    return out;
}

void receive_data(uint8_t addr, uint8_t *data_buffer, uint8_t bytesToBeReceived)
{
    const uint8_t bytesReceived = Wire.requestFrom(addr, bytesToBeReceived);
    for (int i = 0; i < bytesReceived; i++)
    {
        uint8_t dataRead = Wire.read();
        data_buffer[i] = dataRead;
    }
}

void intToBytesArray(int value, uint8_t bytes[4])
{
    
    for (int i = 0; i < int(sizeof(value)); i++)
    {
        bytes[i] = (value & (0xff << ( i * 8))) >> 8*i;
    } 

}

int bytesArraytoInt(volatile uint8_t *data, uint8_t len, uint8_t begin_val)
{
    int value = 0;
    int buffer = 0;
    for (int i = begin_val; i < (len + begin_val); i++)
    {
        buffer = data[i];
        Serial.println(data[i]);
        buffer = buffer << ((i - begin_val) * 8);
        value += buffer;
    }
    return value;
}


void check_ble(device * nodes[10], uint8_t n){
    if (deviceConnected)
        {
            dataToSend=data_to_send(nodes,n);
            pCharacteristic->setValue(dataToSend);
            pCharacteristic->notify();
            delay(500); // Adjust delay as needed
        }

        if (!deviceConnected && oldDeviceConnected)
        {
            delay(500);
            pServer->startAdvertising();
            Serial.println("start advertising");
            oldDeviceConnected = deviceConnected;
        }

        if (deviceConnected && !oldDeviceConnected)
        {

            oldDeviceConnected = deviceConnected;
        }
}

int init_ble(){
  BLEDevice::init(DEV_NAME);
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_INDICATE |
          BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();

  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();
  Serial.println("Waiting for client connection to notify...");
  return 1;
}

std::string data_to_send(device *nodes[10], uint8_t n)
{
    std::string data = "{\"Device\":\"";
    // on écrit du json en flatten
    /* {"Device" : DEV_NAME,
         "Nodes" : {
            "type" : node[i]->type,
            "id":  node[i]->id,
            "produced subjects": {
                nodes[i]->my_subjects[j]->id : nodes[i]->my_subjects[j]->value
            }
            "received subjects": {
                nodes[i]->my_subjects[j]->id : nodes[i]->my_subjects[j]->value
            }
         }
        }
    */
    data.append(DEV_NAME);
    data.append("\",\"Nodes\":\{ \"");

    char buff[50];
    for (int i = 0; i < n; i++)
    {
        sprintf(buff,"%d",i);
        data.append(buff);
        data.append("\":{ \"type\":");


        sprintf(buff,"%d",nodes[i]->type);
        data.append(buff);

        data.append(",\"id\":");
        sprintf(buff,"%d",nodes[i]->id);
        data.append(buff);
        data.append(",\"Produced subject\":{\"");
        sprintf(buff,"%d",nodes[i]->my_subjects[1]->id);
        data.append(buff);        
        data.append("\":");
        sprintf(buff,"%d",nodes[i]->my_subjects[1]->value);
        data.append(buff);
        data.append("}");
        data.append(",\"Received subject\":{ \"");
        sprintf(buff,"%d",nodes[i]->my_subjects[0]->id);
        data.append(buff);
        data.append("\":");
        sprintf(buff,"%d",nodes[i]->my_subjects[0]->value);
        data.append(buff);
        data.append("}}");
    }

    data.append("}}");

    return data;
}