#include "device_handler.h"

int bytesArraytoInt(uint8_t *data, uint8_t len, uint8_t begin_val)
{
  int value = 0;
  int buffer = 0;
  for (int i = begin_val; i < len; i++)
  {
    buffer = data[i];
    buffer = buffer << ((i - begin_val) * 8);
    value += buffer;
  }
  Serial.println("extracted value from data received : ");
  Serial.println(value);

  return value;
}

DeviceHandler::DeviceHandler()
{
    this->list_dev = NULL;
    this->size_buffer = 0;
    this->size = 0;
}

/* DeviceHandler::DeviceHandler(list_device* list_dev, uint8_t size){
  this->list_dev=NULL;
  for (int i=0;i<size;i++){
    push(&(this->list_dev),);
  }
  this->size_buffer=0;
  this->size=size;
} */

DeviceHandler::~DeviceHandler()
{
    free(this->list_dev);
}

void DeviceHandler::add_new_device(uint8_t addr,uint8_t * buffer)
{
    if (this->size < MAX_DEVICE)
    {
        Serial.println("there is enought place for the device");
        Serial.println("Device is an actuator");
        uint8_t subscription=buffer[3];
        uint8_t behaviour = buffer[4];

        int id = buffer[1];;
        int tmp = buffer[2];;
        tmp = tmp << ((2-1) * 8);
        id += tmp;


        push(&(this->list_dev), new Device(addr, id,subscription, behaviour));
        Serial.println("Device has been saved");
        Serial.println("Device type : ");
        Serial.println(access(&list_dev, addr)->type);
        Serial.println("Device addr : ");
        Serial.println(access(&list_dev, addr)->addr);
        Serial.println("Device id : ");
        Serial.println(access(&list_dev, addr)->id);
        Serial.println("Device subscriptions : ");
        Serial.println(access(&list_dev, addr)->subscription);
        Serial.println("Device subscriptions : ");
        Serial.println(access(&list_dev, addr)->current_behaviour);
    }
    this->size += 1;
};

uint8_t DeviceHandler::ask_free_addr()
{
    if (this->size == 0)
    {
        return 0x09;
    }
    return 0x09 + this->size;
};

void DeviceHandler::filter_list(const char *filter)
{
    this->size_buffer = 0;
    Device *tmp = NULL;
    if (!(strcmp(filter, "sensor")))
    {
        for (int i = 0; i < this->size; i++)
        {
            tmp = access(&list_dev, 0x09 + i);
            if (tmp->type == 0)
            {
                this->filter_buffer[i] = tmp;
                this->size_buffer += 1;
            }
        }
    }
    else if (!(strcmp(filter, "actuator")))
    {
        for (int i = 0; i < this->size; i++)
        {
            tmp = access(&list_dev, 0x09 + i);
            if (tmp->type == 1)
            {
                this->filter_buffer[i] = tmp;
                this->size_buffer += 1;
            }
        }
    }
}

Device * DeviceHandler::access_dev(uint8_t i){
    return access(&list_dev, 0x09+i);
}

void DeviceHandler::update_value(uint8_t addr, SlaveResponse resp)
{
    Serial.println("updtating value of addr : ");
    Serial.println(addr);
    Device *tmp = NULL;
    int buf=0;
    tmp = access(&list_dev, addr);
    if (tmp)
    {
        if (resp.buffer[0]==2)
        {
            tmp->current_value = tmp->previous_value;
        }
        else
        {   // à modifier pour plus d'abstraction
            tmp->status = resp.buffer[1];
            if (tmp->sub_type==1){
                //accelero 
                tmp->prev_x=tmp->x;
                buf = bytesArraytoInt(resp.buffer,6,2);
                tmp->x=reinterpret_cast<float&>(buf);
                tmp->prev_y=tmp->y;
                buf = bytesArraytoInt(resp.buffer,10,6);
                tmp->y=reinterpret_cast<float&>(buf);
            }
            else {
                tmp->previous_value=tmp->current_value;     
                tmp->current_value=bytesArraytoInt(resp.buffer,6,2);
            }
        }
    }
}

/* void DeviceHandler::put_in_quarantine(uint8_t addr)
{
    Serial.println("putting in quarantine the device at addr : ");
    Serial.println(addr);
    Device *tmp = NULL;
    tmp = access(&list_dev, addr);
    if (tmp)
    {
        tmp->quarantine = 1;
        this->check_device_flag = 1;
    }
} */

void DeviceHandler::delete_device(uint8_t addr)
{
    retreive(&list_dev, addr);
    this->size -= 1;
    this->check_device_flag = 0;
}
