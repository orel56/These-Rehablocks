#include "actuator.h"

Actuator::Actuator(){};

void Actuator::process(volatile uint8_t * buffer, uint8_t len){
    Device::process(buffer,len); 
    if (command=="set_value"){
      this->set_value(bytesArraytoInt(buffer,len));
          }
    else {

    }
}

bool Actuator::is_setable (int value){
    return true;
}
bool Actuator::set_value (int value){
        return true;
}

int Actuator::bytesArraytoInt(volatile uint8_t* data,uint8_t len){
    int value =0;
    int buffer = 0;
    for (int i=1;i<len;i++){
          buffer = data[i];
          buffer = buffer<<((i-1)*8);      
        value+=buffer;
    }
    return value;
}

SlaveResponse Actuator::getResponse(){
    SlaveResponse response;
    
    response = Device::getResponse();

    if(command=="set_value"){
       if(is_set){
        response.buffer[0]=0x01;
       }
       else{
        response.buffer[0]=0x00;
       }
    }
    return response;
}
