/* MyI2CPeripheral
 * A class to centralize our peripheral's state 
 * and message processing.
 */

#include "MyI2Cdevice.h"
#include "config.h"
#include <Wire.h>
char*mycommands[6]={strdup("connexion"),strdup("change_addr"),strdup("get_value"),strdup("green_led"),strdup("red_led"),strdup("get_type")};

uint16_t hex_val;

SlaveResponse MyI2CPeripheral::getResponse(int val){
    SlaveResponse response;
    Serial.print("command in get response : ");
    Serial.println(command);
    if (strcmp(command,"get_value")==0){
       uint8_t value_octet[2];
       value_octet[0]=(uint8_t) (val & 0xff) ;
       value_octet[1]=(uint8_t)( (val >> 8));
       response.buffer[0]= value_octet[0];
       response.buffer[1]= value_octet[1];
       Serial.println(val);
       response.size=2;
       return response;
    }
    else if (strcmp(command,"get_type")==0){

      response.buffer[0]=(uint8_t) Block_type;
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

uint8_t MyI2CPeripheral::expectedReceiveLength(uint8_t forRegister){
  if (forRegister!=0x01){
    return 1;
  }
  else {return 2;}
};

void MyI2CPeripheral::process(volatile uint8_t * buffer, uint8_t len){
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

void MyI2CPeripheral::doThings(int* val){
    *val = analogRead(A0);
};

MyI2CPeripheral::MyI2CPeripheral(){};

addrTab MyI2CPeripheral::scan(){
  byte error, address;
  addrTab findAddr;

  Serial.println("Scanning...");

  findAddr.size = 0;
  for(address = 8; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
      findAddr.addrs[findAddr.size]=address;
      findAddr.size++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  return findAddr;

};


void MyI2CPeripheral::changeAddr(addrTab usedAddr){
  int addr=random(8,127);
  bool valid=0;
  while(!valid){
    addr=random(8,127);

    for(int k=0;k<usedAddr.size;k++){

      if (usedAddr.addrs[k]==addr){
        valid=0;
        break;
      }
      else if ((k+1)==usedAddr.size){
        valid=1;
      }
    }
  }
  my_addr=addr;
  TWAR=addr<<1;

};

void MyI2CPeripheral::sendReady(){
  uint8_t data[1]={0x1a};

  Wire.write(data,1);
}