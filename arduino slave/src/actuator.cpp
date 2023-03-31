#include "actuator.h"

Actuator::Actuator(){};

void Actuator::process(volatile uint8_t * buffer, uint8_t len){
    Device::process(buffer,len); 
    this->command=this->mycommands[buffer[0]];
    if (command=="set_value"){
        Serial.println("command set_value is detected");
      this->set_value(bytesArraytoInt(buffer,len));
          }
    else {
        Serial.println("command set_value is not detected");

    }
}

bool Actuator::is_setable (int value){        
    Serial.println("wrong is setable...");

    return true;
}
bool Actuator::set_value (int value){
        return true;
}
uint8_t Actuator::expectedReceiveLength(uint8_t forRegister){
  if (forRegister==0x01){
    return 2;
  }
  else if (forRegister==0x03){
    Serial.println("for command set_value 5 bytes are expected");

    return 5;
  }
  else {return 1;}
};
int Actuator::bytesArraytoInt(volatile uint8_t* data,uint8_t len){
    int value =0;
    int buffer = 0;
    for (int i=1;i<len;i++){
          buffer = data[i];
          buffer = buffer<<((i-1)*8);      
        value+=buffer;
    }
    Serial.println("extracted value from data received : ");
    Serial.println(value);


    return value;
}

SlaveResponse Actuator::getResponse(){
    SlaveResponse response;
    
    response = Device::getResponse();

    if ( command=="get_info"){
      response.buffer[0]=0x01;
      Serial.println(response.buffer[0]);
      response.buffer[1]=1;
      response.size =2;
      return response;

    }
    else if(command=="set_value"){
       if(is_set){
        response.buffer[0]=0x01;
       }
       else{
        response.buffer[0]=0x00;
       }
    }
    return response;
}
