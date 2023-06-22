#include <Wire.h>
#include "config.h"
#include "device_factory.h"
#include <EEPROM.h>
#include <Arduino.h>


void i2cRequestEvent()
{  
  I2CDevice->i2cRequest();
}

void i2cReceiveEvent(int bytesReceived)
{
  I2CDevice->i2cReceive(bytesReceived);
}

void setup() {
  // SETUP wire
  pinMode(USR_LED,OUTPUT);
  pinMode(SDAP,INPUT);
  pinMode(SAP,OUTPUT);

  EEPROM.begin();
  Serial.begin(9600);
  //pas LOW plutot du analog read
  if (analogRead(SDAP)==0){
    I2CDevice->mode=1;
    digitalWrite(USR_LED,LOW);
    pinMode(DECO_BTN,INPUT_PULLUP);
  }
  else {
    byte i2c_addr;
    i2c_addr = EEPROM.read(0x00);
    Wire.begin(i2c_addr);
    Wire.onRequest(i2cRequestEvent);
    Wire.onReceive(i2cReceiveEvent);
    if (i2c_addr==0x08){
      I2CDevice->mode=0;
      digitalWrite(USR_LED,HIGH);
      digitalWrite(SAP,HIGH);

    }
    else{
      I2CDevice->mode=2;
      digitalWrite(USR_LED,LOW);
    } 

  }
 
  // do other setup you may need...
}
/*
 * main loop.
 * Here, we'll just loop around and handle 
 * pending commands when they come in.
 */
void loop() {
if (I2CDevice->mode ==1){
  I2CDevice->deconnect();
}
else if(I2CDevice->mode==2){
  I2CDevice->behav();
}
else if (I2CDevice->mode==3){
  Serial.println("Deconexion done, device can be unpluged and used");
  delay(1000);
}
}