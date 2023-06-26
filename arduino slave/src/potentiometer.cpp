#include "potentiometer.h"

Potentiometer::Potentiometer(){

}


Potentiometer::Potentiometer(int adc_pin, int threshold, int id){
    this->adc_pin = adc_pin;
    this->id = id;
    this->threshold=threshold;
    pinMode(adc_pin, INPUT);
}

void Potentiometer::behaviour1(){
    int read =analogRead(adc_pin);
    if(abs(this->current_value - read)>this->threshold){
        this->previous_value=this->current_value;
        this->current_value=read;
    }
}

void Potentiometer::update_param(){
     if(this->pendingCommand[1]==1){
        this->threshold=bytesArraytoInt(pendingCommand,5,2);
    }
}

void Potentiometer::update_subject(){
   
}

void Potentiometer::get_status(){
    SlaveResponse resp;
    resp.buffer[0]=this->acknowledge;
    resp.buffer[1]=0;
    uint8_t* value = int_to_bytesarray (this->current_value);
    resp.buffer[2] = value[0];
    resp.buffer[3] = value[1];
    resp.buffer[4] = value[2];
    resp.buffer[5] = value[3];
    resp.size=6;
    this->status=resp;

}