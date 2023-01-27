#include <Arduino.h>
#include <Wire.h>
int val=0;
uint8_t buff[20];
uint8_t data[3];
uint16_t val_potar;
uint8_t addr=0x08;
int connected = 0;

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



void setup() {
  Serial.begin(9600);
  Wire.begin();
}

/* void loop() {

  if (addr==0x08){
   data[0]=0x00;
   Serial.printf("Sending command 0x00 to 0x08 device...");
   int out = send_command(0x08,data,1);
   Serial.print("transmission output : ");
   Serial.println(out);

   if (out == 0) {
    Serial.printf("Receiving data must be ...");
    receive_data(0x08,buff,1);
    Serial.printf("printing data received : ");
    val_potar= buff[1]<<8;
    val_potar=val_potar | ((uint16_t) buff[0]);
    
    Serial.println(buff[0],BIN);
    Serial.println(buff[1],BIN);

    Serial.println(val_potar,BIN);
    Serial.println(val_potar);

   }
   else {Serial.println("something went wrong"); 
        }
   delay(100);
  }

  }*/

void loop() {

  if (! connected){
   Serial.println("0");
   data[0]=0x00;
   int out = send_command(addr,data,1);

   if (out == 0) {
    receive_data(addr,buff,1);
    if (buff[0]==0x01){
      connected=1;
    }

   }
   else {
        connected=0;
        }
   delay(100);
  }
  else if (connected == 1){
   Serial.println("1");

   data[0]=0x01;
   data[1]=0x0a;
   Serial.println(addr);
   int out = send_command(addr,data,2);
    delay(5);
   if (out == 0) {
    addr=0x0a;
    receive_data(addr,buff,1);
    if (buff[0]==0x02){
      connected=2;
    }
   }
   else {
        connected=0; 
        }
   delay(100);
  }
  else {
   data[0]=0x02;
   int out = send_command(addr,data,1);

   if (out == 0) {
    receive_data(addr,buff,2);
    val_potar= buff[1]<<8;
    val_potar=val_potar | ((uint16_t) buff[0]);
    Serial.println(val_potar);
   }
   else {
        connected=0;
        }
   delay(100);

    
  }
  delay(100);

  }