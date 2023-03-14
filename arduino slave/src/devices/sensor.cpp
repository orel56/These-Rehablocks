#include "sensor.h"

Sensor::Sensor(){}

void Sensor::process(volatile uint8_t * buffer, uint8_t len){
    Device::process(buffer,len);
    if (strcmp(command,"get_value")==0){
      this->get_value();
          }
    else {

    }
}

bool Sensor::get_value (){
    return true;
}

bool Sensor::check_value (int value){
    return true;
}

unsigned char* int_to_bytesarray(int value){

    static unsigned char bytes[sizeof(value)];
    for(unsigned int i=0;i<sizeof(value);i++){
        bytes[i]=(value & (0x000000ff << (2*i*4))) >> 8*i;
    }
    return bytes;
}

SlaveResponse Sensor::getResponse(){
    SlaveResponse response;
    
    response = Device::getResponse();

    if(strcmp(command,"get_value")==0){
       if (send_value){
       unsigned char* my_value_in_bytes;
       my_value_in_bytes=int_to_bytesarray(current_value);
       response.size=sizeof(current_value) +1;
       response.buffer[0]=0x01;

       for(unsigned int i=1;i<response.size;i++){
        response.buffer[i]=my_value_in_bytes[i-1];
       }
       }
       else{
        response.buffer[0]=0x00;
       }
    }
    return response;

}

uint8_t Sensor::expectedReceiveLength(uint8_t forRegister){
   return Device::expectedReceiveLength(forRegister);
};

void Sensor::doThings(){

};

