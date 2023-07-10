#include <device.h>


Device::Device(){
    this->addr=0x08;
    this->current_value=0;
    this->type=2;
};

Device::Device(uint8_t addr, int id,uint8_t subscription, uint8_t behaviour){
    this->addr=addr;
    this->current_value=0;
    this->id=id;
    this->extract_id_info();
    this->subscription=subscription;
    this->current_behaviour=behaviour;
}


void Device::extract_id_info(){
    this->type=(this->id)>>15;
    this->sub_type=(this->id)>>10 & 0b011111;
    this->familly=(this->id)>>7 & 0b000000111;
    this->sub_fam=(this->id)>>4 & 0b000000000111;
}

bool Device::is_subscribe(uint8_t id){
    return this->subscription & id;
}
