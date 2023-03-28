#include <device.h>

Device::Device(){
    this->addr=0x08;
    this->current_value=0;
    this->quarantine=0;
    this->type=2;
};

Device::Device(uint8_t addr,uint8_t type){
    this->addr=addr;
    this->current_value=0;
    this->quarantine=0;
    this->type=type;
}

Sensor::Sensor(){
    this->to_be_send=0;
};

Actuator::Actuator(){
    this->to_be_set=0;
};

Sensor::Sensor(uint8_t addr,uint8_t type): Device(addr,type){
    this->to_be_send=0;
}

Actuator::Actuator(uint8_t addr,uint8_t type): Device(addr,type){
    this->to_be_set=0;
}