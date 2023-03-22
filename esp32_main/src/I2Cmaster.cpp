/* MyI2CPeripheral
 * A class to centralize our peripheral's state 
 * and message processing.
 */

#include "I2Cmaster.h"

int search_str (char** list_str,char* str,int size_list){
    for (int i=0;i<size_list;i++){
        if(strcmp(list_str[i],str)==0){
            return i;
        }
    }
    return -1;
}

char*mycommands[6]={strdup("ping"),
                    strdup("change_addr"),
                    strdup("get_value"),
                    strdup("green_led"),
                    strdup("red_led"),
                    strdup("get_type")}; 

uint8_t size_list = 6;


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

std::vector<byte> I2Cmaster::scan(){
  byte error, address;
  std::vector<byte> findAddr;
  Serial.println("Scanning...");
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
      findAddr.push_back(address);
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
    uint8_t new_addr= ask_free_addr();
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


void I2Cmaster::i2c_init(){
  Wire.begin();

  std::vector<uint8_t> bus_addrs = this->scan();
  for (int k=0;k<bus_addrs.size();k++){
    byte error =0;
    error=I2Cmaster::ping(bus_addrs[k]);

    if (!error){

        error=I2Cmaster::send_command(bus_addrs[k],"get_info",{},0);

        if(!error){
          I2Cmaster::receive_data(bus_addrs[k]);
          add_new_device(&(I2Cmaster::reponse_buffer),bus_addrs[k]);
        }
    }
  }
  }