/* MyI2CPeripheral
 * A class to centralize our peripheral's state 
 * and message processing.
 */

#include "I2Cmaster.h"
#include "config.h"
#include <Wire.h>
#include "device_handler.cpp"
uint16_t hex_val;

I2Cmaster::I2Cmaster(){};


uint8_t I2Cmaster::send_command(uint8_t addr,const char* command,uint8_t * data, uint8_t bytes){
  int commandCode = search_str(mycommands,strdup(command),size_list);
  if (commandCode==-1){
    return 15;
  }
  else {
  Wire.beginTransmission(addr);
  Wire.write(uint8_t(commandCode));
  //Serial.println(datas[0]);
  for (int i=0; i<bytes;i++){
    //Serial.print(datas[i]);
    Wire.write(data[i]);

  }
  int out=Wire.endTransmission();

  return out;
  }
}

void I2Cmaster::receive_data(uint8_t addr, uint8_t bytesToBeReceived=20){
  const uint8_t bytesReceived=Wire.requestFrom(addr,bytesToBeReceived);
  I2Cmaster::reponse_buffer.size=bytesReceived;
    for (int i=0; i<bytesReceived;i++){
        uint8_t dataRead=Wire.read();
        I2Cmaster::reponse_buffer.buffer[i]=dataRead;
    }
}

addrTab I2Cmaster::scan(){
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

uint8_t I2Cmaster::ping(uint8_t addr){
   byte error;
    Wire.beginTransmission(addr);
    error = Wire.endTransmission();
  return error;
}

void I2Cmaster::apering_process(){
  byte error =0;
  error=I2Cmaster::ping(0x08);

  if (!error){
    //0x08 device received ping
    uint8_t new_addr= ask_free_addr(0x08);
    uint8_t data[0];
    data[0]=new_addr;
    error=I2Cmaster::send_command(0x08,"change_addr",data,1);

    if (!error){
      I2Cmaster::receive_data(new_addr,1);

      if (I2Cmaster::reponse_buffer.buffer[0]==0x01){

          error=I2Cmaster::send_command(new_addr,"get_info",{},0);

          if(!error){
            I2Cmaster::receive_data(new_addr);
            add_new_device(&(I2Cmaster::reponse_buffer),new_addr);
          }
      }
    }
  }
}
 