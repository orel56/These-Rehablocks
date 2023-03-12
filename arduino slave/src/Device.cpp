/* Device
 * A class to centralize our peripheral's state 
 * and message processing.
 */

#include "Device.h"
#include "config.h"
#include <Wire.h>
#include"EEPROM.h"

char*mycommands[3]={strdup("ping"),strdup("change_addr"),strdup("get_info")};

uint16_t hex_val;

SlaveResponse Device::getResponse(){
    SlaveResponse response;
    Serial.print("command in get response : ");
    Serial.println(command);
   if (strcmp(command,"get_info")==0){

     // response.buffer[0]=(uint8_t) Block_type;
      Serial.println(response.buffer[0]);
      response.size =1;
      return response;

    }
    else {
       response.buffer[0]= 0x01;

       response.size=1;
       return response;
    }
  
};

uint8_t Device::expectedReceiveLength(uint8_t forRegister){
  if (forRegister!=0x01){
    return 1;
  }
  else {return 2;}
};

void Device::process(volatile uint8_t * buffer, uint8_t len){
    command=mycommands[buffer[0]];
    Serial.println(command);
   if (strcmp(command,"change_addr")==0){
      TWAR = buffer[1] << 1;
    }
   else if (strcmp(command,"green_led")==0){
    digitalWrite(RED_LED,LOW);
    digitalWrite(LED_BUILTIN,HIGH);
   }
   else if (strcmp(command,"red_led")==0){
    digitalWrite(RED_LED,HIGH);
    digitalWrite(LED_BUILTIN,LOW);
   }
    
};

void Device::doThings(){
};

Device::Device(){};


void Device::changeAddr(uint8_t addr){
  my_addr=addr;
  TWAR=addr<<1;
}

void Device::tick(){

 if (pendingCommandLength) {
    Serial.println("hello there");

    // oh my, we've received a command!
    
    // if you're going to be very slow in processing this,
    // you could copy the contents of pendingCommand[]
    // over to yet another buffer.
    // Here we just do it "real time" for simplicity

    // 1) process that command
    Device::process(pendingCommand, pendingCommandLength);

    // 2) zero that flag, so we don't process multiple times
    pendingCommandLength = 0;

    Device::doThings();

  }

  // do anything else that needs doin'

  // main thing is that you must process any pendingCommand
  // before the next full command bytes come in through the wire

}

void Device::deconnect(){
  bool btn_val= digitalRead(DECO_BTN);

  if ((btn_val!=this->deco_btn) && btn_val){
     EEPROM.write(0x00,0x08);
     digitalWrite(USR_LED,HIGH);
     Serial.println("addr erased, 0x08 wrote in 0x00 eeprom address");
     this->mode = "END";
  }
}

void Device::connect(){
  if (this->connect_follow <3){
      this->tick();
      this->connect_follow ++;}
  else {
    digitalWrite(USR_LED,LOW);
    this->mode="working";
  }
}