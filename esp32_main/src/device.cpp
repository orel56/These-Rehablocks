#include <device.h>

Device::Device(){
    this->addr=0x08;
    this->current_value=0;
    this->quarantine=0;
    this->type=2;
    this->to_be=0;
};

Device::Device(uint8_t addr,uint8_t type){
    this->addr=addr;
    this->current_value=0;
    this->quarantine=0;
    this->type=type;
    this->to_be=0;

}

Sensor::Sensor(){
};

Actuator::Actuator(){
};

Sensor::Sensor(uint8_t addr,uint8_t type,int threshold): Device(addr,type){
    this->threshold=threshold;
}

Actuator::Actuator(uint8_t addr,uint8_t type, uint8_t linked): Device(addr,type){
    this->linked = linked;
}

void Sensor::analyse(){
    this->to_be=(abs(this->previous_value - this->current_value)>this->threshold);
}

void Actuator::analyse(){
    this->to_be=true;
    this->current_value=1;
}