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
  uint8_t msgLen = 0;
  // loop over each incoming byte
  for (int i = 0; i < bytesReceived; i++)
  {
    // stick that byte in our receive buffer
    I2CDevice->receivedBytes[I2CDevice->receivedByteIdx] = Wire.read();

    
    // now, we're sure we have _at least_ one byte in the buffer
    if (! msgLen) {
      // this was the first byte of a message, 
      // so we couldn't know the 
      // expected message length until now... 
      // ask our device what to expect:
      msgLen = I2CDevice->expectedReceiveLength(I2CDevice->receivedBytes[0]);

    }

    I2CDevice->receivedByteIdx++; /* increment in-byte counter */

    if (I2CDevice->receivedByteIdx >= msgLen) {
       
        // we have a complete request/command in our buffer!
        
        // 1) copy that into our pending commands buffer
        // could use memcpy, we do it manual style:
        for (uint8_t i=0; i<msgLen; i++) {
          I2CDevice->pendingCommand[i] = I2CDevice->receivedBytes[i];
        }

        // 2) tell the main loop we've got something 
        // of interest in pending cmd buffer
        I2CDevice->pendingCommandLength = msgLen;
        // 3) zero our in-bytes buffer, to start 
        // the next message
        I2CDevice->receivedByteIdx = 0;

        // 4) zero our expected msgLen, so we'll refresh it
        // for the next command
        msgLen = 0;
    }
  }
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
    I2CDevice->mode="deconnect";
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
      I2CDevice->mode="working";
      digitalWrite(USR_LED,HIGH);
      digitalWrite(SAP,HIGH);

    }
    else{
      I2CDevice->mode="working";
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
if (I2CDevice->mode == "deconnect"){
  I2CDevice->deconnect();

}
else{
  Serial.println("Deconexion done, device can be unpluged and used");
  delay(1000);
}
}