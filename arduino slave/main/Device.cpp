/* Device
 * A class to centralize our peripheral's state 
 * and message processing.
 */

#include "Device.h"
#include "config.h"
#include <Wire.h>
#include"EEPROM.h"

uint16_t hex_val;

SlaveResponse Device::getResponse(){
    SlaveResponse response;
    if ( (command=="ping") ||  (command=="change_addr")) {
       response.buffer[0]= 0x01;
       response.size=1;
       return response;
    }
    return response;
  
};

uint8_t Device::expectedReceiveLength(uint8_t forRegister){
  if (forRegister!=0x01){
    return 1;
  }
  else {return 2;}
};

void Device::process(volatile uint8_t * buffer, uint8_t len){
   this->command=this->mycommands[buffer[0]];
   if ( (command=="change_addr")){
      Serial.println("command is change addr");
      this->connect_follow++;
      this->changeAddr(buffer[1]);
          }
   else if (( (command=="ping")) && (this->my_addr==0x08)){
      Serial.println("command is ping and my addr is 0x08 meaning I 'am connecting");
      this->mode="connect";
      this->connect_follow++;
   }
   else if ( (command=="get_info") && (this->mode=="connect")){
      Serial.println("command is get_info");

      this->connect_follow++;

   }
};

void Device::doThings(){
};

Device::Device(){
  this->my_addr=EEPROM.read(0x00);
};


void Device::changeAddr(uint8_t addr){
  Serial.println("changing my addr 0x08 for the new one : ");
  Serial.println(addr);
  this->my_addr=addr;
  TWAR=addr<<1;
  EEPROM.write(0x00,this->my_addr);
}

void Device::tick(){
 if (pendingCommandLength) {
    // oh my, we've received a command!
    
    // if you're going to be very slow in processing this,
    // you could copy the contents of pendingCommand[]
    // over to yet another buffer.
    // Here we just do it "real time" for simplicity

    // 1) process that command
    Serial.println("there a new pending command that need to be process");
    process(pendingCommand, pendingCommandLength);

    // 2) zero that flag, so we don't process multiple times
    pendingCommandLength = 0;

    doThings();

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
      this->tick();}
  else {
    digitalWrite(USR_LED,LOW);
    Serial.println("connect end");
    digitalWrite(SAP,LOW);
    this->mode="working";
  }
}

void Device::i2cRequest(){
  if (this->mode =="connect")
  {
    this->connect();

  }
  else if (this->mode =="working") {
    this->tick();
  }
   // get the response (the I2CDevice knows what to say)
   SlaveResponse resp = this->getResponse();
   // write it to the out buffer

   Wire.write(resp.buffer, resp.size); 
}