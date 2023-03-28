#include "sensor.h"

Sensor::Sensor(){}

void Sensor::process(volatile uint8_t * buffer, uint8_t len){
    Device::process(buffer,len);
    this->command=Sensor::mycommands[buffer[0]];
    if (this->command=="get_value"){
      this->get_value();
          }
}

bool Sensor::get_value (){
    return true;
}

bool Sensor::check_value (int value){
    return true;
}

uint8_t * Sensor::int_to_bytesarray(int value){

    static unsigned char bytes[sizeof(value)];
    for(unsigned int i=0;i<sizeof(value);i++){
        bytes[i]=(value & (0x000000ff << (2*i*4))) >> 8*i;
    }
    return bytes;
}

SlaveResponse Sensor::getResponse(){
    SlaveResponse response;
    response = Device::getResponse();
     if ( command=="get_info"){
      response.buffer[0]=0x01;
      Serial.println(response.buffer[0]);
      response.buffer[1]=0;
      response.size =2;
      return response;

    }
    else if(command=="get_value"){

       if (send_value){
       unsigned char* my_value_in_bytes;
       my_value_in_bytes=int_to_bytesarray(current_value);
       response.size=5;
       response.buffer[0]=0x01;

       for(unsigned int i=1;i<5;i++){
        if (i<(sizeof(current_value) +1)){
        response.buffer[i]=my_value_in_bytes[i-1];}
        else {
            response.buffer[i]=0;
        }
        Serial.println(response.buffer[i],BIN);

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

