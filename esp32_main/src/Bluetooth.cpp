#include <Bluetooth.h>

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

void check_ble(Device * nodes[10], uint8_t n){
    if (deviceConnected)
        {
            dataToSend=data_to_send(nodes,n);
            pCharacteristic->setValue(dataToSend);
            pCharacteristic->notify();
            delay(50); // Adjust delay as needed
        }

        if (!deviceConnected && oldDeviceConnected)
        {
            delay(50);
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

std::string data_to_send(Device *nodes[10], uint8_t n)
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
    data.append("\",\"Nodes\":\{");

    char buff[10];
    for (int i = 0; i < n; i++)
    {   data.append(" \"");
        sprintf(buff,"%d",i);
        data.append(buff);
        data.append("\":{ \"type\":");


        sprintf(buff,"%d",nodes[i]->type);
        data.append(buff);

        data.append(",\"id\":");
        sprintf(buff,"%d",nodes[i]->id);
        data.append(buff);
        data.append(",\"address\":");
        sprintf(buff,"%d",nodes[i]->addr);
        data.append(buff);
        data.append(",\"Produced subject\":{\"");
        sprintf(buff,"%d",nodes[i]->subjects[0]->id);
        data.append(buff);        
        data.append("\":");
        sprintf(buff,"%d",nodes[i]->subjects[0]->value);
        data.append(buff);
        data.append("}");
        data.append(",\"Received subject\":{ \"");
        sprintf(buff,"%d",nodes[i]->subjects[1]->id);
        data.append(buff);
        data.append("\":");
        sprintf(buff,"%d",nodes[i]->subjects[1]->value);
        data.append(buff);
        data.append("}},");
    }
    data.pop_back();
    data.append("}}");

    return data;
}