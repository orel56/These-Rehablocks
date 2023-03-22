#include <Arduino.h>
#include <Wire.h>
#include <config.h>

uint8_t buff[20];

uint8_t data[3];
volatile uint8_t receivedBytes[RECEIVED_COMMAND_MAX_BYTES];
int val_potar;

bool change  = 1;

Tab findAddr;

slaveList listSlave;

int send_command(uint8_t addr,uint8_t * datas,const uint8_t bytes) {
  Wire.beginTransmission(addr);
  //Serial.println(datas[0]);
  for (int i=0; i<bytes;i++){
    //Serial.print(datas[i]);
    Wire.write(data[i]);

  }
  
  int out=Wire.endTransmission();

  return out;
}

void receive_data(uint8_t addr,uint8_t * data_buffer, uint8_t bytesToBeReceived){
    const uint8_t bytesReceived=Wire.requestFrom(addr,bytesToBeReceived);
    for (int i=0; i<bytesReceived;i++){
        uint8_t dataRead=Wire.read();
        data_buffer[i]=dataRead;
    }

}

/* bool scan(Tab *slaveAddr){
  byte error, address;
  int chg=1;
  Tab newAddr;
  Serial.println("Scanning...");

  newAddr.size=0;
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
      newAddr.addrs[newAddr.size]=address;
      newAddr.size++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  Serial.println("on sort ");

  if(slaveAddr->size <= newAddr.size){
    chg=memcmp(newAddr.addrs,slaveAddr->addrs,slaveAddr->size);
  }
  else {
    chg=memcmp(newAddr.addrs,slaveAddr->addrs,newAddr.size);
  }

  *slaveAddr=newAddr;

  return chg;
}; */


/* void receive_ready(int bytes){
 for (int i = 0; i < bytes; i++)
  {
    // stick that byte in our receive buffer
  receivedBytes[i] = Wire.read();
  Serial.println(receivedBytes[i]);
  }

  if (receivedBytes[0]==26){
    change=scan(&findAddr);
  }

} */

void request(){}


/* bool checkAddr(uint8_t addr,slaveList slaves){
  for (int k=0;k<slaves.size;k++){
    if (slaves.list[k].addr==addr){
      return true;
    }
  }
  return false;
} */


void setup() {
  Serial.begin(9600);
  Wire.begin();

}
 void loop() {
if (change==0){
   data[0]=0x00;
   Serial.printf("Sending command 0x00 to 0x08 device...");
   int out = send_command(0x08,data,1);
   if (out == 0) {
    data[0]=0x01;
    data[1]=0x0b;
    out= send_command(0x08,data,2);
    if (out==0){
    receive_data(0x0b,buff,1);
    if (buff[0]==0x01){
      data[0]=0x02;
      out= send_command(0x0b,data,1);
      if (out==0){
        receive_data(0x0b,buff,1);
         if (buff[0]==0x01){
          change=1;}
          else{
            Serial.println("wrong output for get_info");
          }
     }
        else{
            Serial.println("get_info didn't reach device");
          }
    
    }
    else{
            Serial.println("wrong output for change_addr");
          }

   }
    else{
            Serial.println("change_addr didn't reach device");
          }
   delay(100);
  }          
  else{
            Serial.println("ping didn't reach device");
          }
  
  }
  else {
    int x=0;
    val_potar=0;
    data[0]=0x03;
    int out= send_command(0x0b,data,1);
    if (out==0){
    receive_data(0x0b,buff,5);
    Serial.println(buff[0]);

    if (buff[0]==0x01){
        for (int i=1;i<5;i++){
          x = buff[i];
          x = x<<((i-1)*8);
          val_potar+=x;
      }

      Serial.println(val_potar);
  }
      else{
            Serial.println("wrong answer for get_value");
          }    
    }
  else{
            Serial.println("get_value didn't reach device");
          }
  delay(1000);

  }}

/* void loop() {

  if (!cpt){
    Serial.print("loop");
    change=scan(&findAddr);
  if (change){
    Serial.println(findAddr.size);
    for(int k=0;k<findAddr.size;k++){
      if (!checkAddr(findAddr.addrs[k],listSlave))
      {
        slave newslave;
        newslave.addr=findAddr.addrs[k];
        data[0]=0x05;
        int out = send_command(newslave.addr,data,1);
        if (out == 0) {
            receive_data(newslave.addr,buff,2);
            newslave.type=buff[1];
        }
        else {
          Serial.println("error with slave : ");
          Serial.println(newslave.addr);
          Serial.println("new slave does not respond");
        }

        listSlave.list[listSlave.size + 1]=newslave;
        listSlave.size++;
      }
    }
  }
  else {
    Serial.println("everything is fine");
  }
  cpt++;
  }
  else{
    cpt++;
    if (cpt==100000){
      cpt=0;
    }
  }
  for(int k=0;k<listSlave.size;k++){
      if (listSlave.list[k].type){
        data[0]=0x02;
        int out = send_command(listSlave.list[k].addr,data,1);
        if (out == 0) {
            receive_data(listSlave.list[k].addr,buff,2);
            val_potar= buff[1]<<8;
            val_potar=val_potar | ((uint16_t) buff[0]);     
            Serial.println(val_potar);   
            }

        else {
          Serial.println("error with slave : ");
          Serial.println(listSlave.list[k].addr);
          Serial.println("new slave does not respond");
        }
      }
      else {
        if(val_potar>200){
          data[0]=0x03;
        }
        else {
          data[0]=0x04;

        }
        int out = send_command(listSlave.list[k].addr,data,1);
        if (out == 0) {
            receive_data(listSlave.list[k].addr,buff,1);  
            }

        else {
          Serial.println("error with slave : ");
          Serial.println(listSlave.list[k].addr);
          Serial.println("new slave does not respond");
        }

      }
    }  
    
  /* data[0]=0x05;
   int out = send_command(addr,data,1);
   if (out == 0) {
      receive_data(addr,buff,2);
   }
    else {Serial.println("not received");}
   
   for (int k=0; k<2;k++){
    delay(10);
    Serial.println("buffer i");
    Serial.println(buff[k]);

   }*/

  