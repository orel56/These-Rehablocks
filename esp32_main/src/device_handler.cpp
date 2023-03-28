#include "device_handler.h"


DeviceHandler::DeviceHandler(){
  this->list_device=(Device**) malloc(sizeof(Device*)*MAX_DEVICE);
  this->size_buffer=0;
  this->size=0;
}


DeviceHandler::DeviceHandler(Device** list_device, uint8_t size){
  this->list_device=(Device**) malloc(sizeof(Device*)*MAX_DEVICE);
  for (int i=0;i<size;i++){
    this->list_device[i]=list_device[i];
  }
  this->size_buffer=0;
  this->size=size;
}

DeviceHandler::~DeviceHandler(){
    free(this->list_device);
}

void DeviceHandler::add_new_device(uint8_t addr,uint8_t type,int other){
    if (this->size<MAX_DEVICE){
        if(type){
        this->list_device[this->size]=new Actuator(addr,type,(uint8_t) other);
        }
        else {
        this->list_device[this->size]=new Sensor(addr,type,other);
        }
    }
    this-> size+=1;
};
 
uint8_t DeviceHandler::ask_free_addr(){
    if (this->size==0){
        return 0x09;
    }
    return 0x09+this->size;
}; 

void DeviceHandler::filter_list (const char* filter){
    if (!(strcmp(filter,"sensor"))){
        for (int i=0;i<this->size;i++){
            if ((this->list_device[i])->type==0){
                this->filter_buffer[i]=(this->list_device[i])->addr;
            }
        }
    }
    else if (!(strcmp(filter,"actuator"))){
        for (int i=0;i<this->size;i++){
            if ((this->list_device[i])->type==1){
                this->filter_buffer[i]=(this->list_device[i])->addr;
            }
        }
}
}

void DeviceHandler::update_value (uint8_t addr, int value, bool previous){
    for (int i=0;i<this->size;i++){
        if((this->list_device[i])->addr==addr){
           if (previous){
            this->list_device[i]->current_value=this->list_device[i]->previous_value;
           }
           else {
           this->list_device[i]->previous_value=this->list_device[i]->current_value;
           this->list_device[i]->current_value=value;}
            break;
        }
    }
}

void DeviceHandler::put_in_quarantine(uint8_t addr){
    for (int i=0;i<this->size;i++){
        if((this->list_device[i])->addr==addr){
           this->list_device[i]->quarantine=1;
            break;
        }
    }
}


void DeviceHandler::delete_device(uint8_t addr){
    for (int i=0;i<this->size;i++){
        if((this->list_device[i])->addr==addr){
            delete (this->list_device[i]);
            this->list_device[i]=NULL;
            break;
        }
    }
}
