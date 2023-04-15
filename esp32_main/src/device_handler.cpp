#include "device_handler.h"

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

void DeviceHandler::add_new_device(uint8_t addr, uint8_t id, uint8_t linkId, int other)
{
    if (this->size < MAX_DEVICE)
    {
        Serial.println("there is enought place for the device");
        if (id>>7)
        {
            Serial.println("Device is an actuator");

            push(&(this->list_dev), new Actuator(addr, id,linkId, (uint8_t) other));
            Serial.println("Device has been saved");
            Serial.println("Device type : ");
            Serial.println(access(&list_dev, addr)->type);
            Serial.println("Device addr : ");
            Serial.println(access(&list_dev, addr)->addr);
        }
        else
        {
            push(&(this->list_dev), new Sensor(addr, id,linkId, other));
            Serial.println("Device has been saved");
            Serial.println("Device type : ");
            Serial.println(access(&list_dev, addr)->type);
            Serial.println("Device addr : ");
            Serial.println(access(&list_dev, addr)->addr);
        }
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

void DeviceHandler::update_value(uint8_t addr, int value, bool previous)
{
    Serial.println("updtating value of addr : ");
    Serial.println(addr);
    Device *tmp = NULL;
    tmp = access(&list_dev, addr);
    if (tmp)
    {
        if (previous)
        {
            tmp->current_value = tmp->previous_value;
        }
        else
        {
            tmp->previous_value = tmp->current_value;
            tmp->current_value = value;
        }
    }
}

void DeviceHandler::put_in_quarantine(uint8_t addr)
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
}

void DeviceHandler::delete_device(uint8_t addr)
{
    Serial.println("device was in quarantine is it ok ? ");
    retreive(&list_dev, addr);
    this->size -= 1;
    this->check_device_flag = 0;
}
