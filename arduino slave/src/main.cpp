#include <Wire.h>
#include "MyI2Cdevice.h"
#include "config.h"


MyI2CPeripheral I2CDevice = MyI2CPeripheral();

volatile uint8_t receivedBytes[RECEIVED_COMMAND_MAX_BYTES];
volatile uint8_t receivedByteIdx = 0;

volatile uint8_t pendingCommand[RECEIVED_COMMAND_MAX_BYTES];
volatile uint8_t pendingCommandLength = 0;

int val=0;

long cpt=0;

bool scanDone=false;

addrTab findAddr;

/* 
 *  i2cRequestEvent 
 *  this will be called when the master is asking 
 *  to get some data from our peripheral device.20
 */
void i2cRequestEvent()
{
   // get the response (the I2CDevice knows what to say)
   SlaveResponse resp = I2CDevice.getResponse(val);
   uint8_t buf[2]={resp.buffer[0],resp.buffer[1]};
   // write it to the out buffer
   Wire.write(buf, resp.size); 

}


void i2cReceiveEvent(int bytesReceived)
{
  uint8_t msgLen = 0;
  // loop over each incoming byte
  for (int i = 0; i < bytesReceived; i++)
  {
    // stick that byte in our receive buffer
    receivedBytes[receivedByteIdx] = Wire.read();

    
    // now, we're sure we have _at least_ one byte in the buffer
    if (! msgLen) {
      // this was the first byte of a message, 
      // so we couldn't know the 
      // expected message length until now... 
      // ask our device what to expect:
      msgLen = I2CDevice.expectedReceiveLength(receivedBytes[0]);
      Serial.print("msgLen : ");
      Serial.println(msgLen);
    }

    receivedByteIdx++; /* increment in-byte counter */

    if (receivedByteIdx >= msgLen) {
       
        // we have a complete request/command in our buffer!
        
        // 1) copy that into our pending commands buffer
        // could use memcpy, we do it manual style:
        for (uint8_t i=0; i<msgLen; i++) {
          pendingCommand[i] = receivedBytes[i];
        }

        // 2) tell the main loop we've got something 
        // of interest in pending cmd buffer
        pendingCommandLength = msgLen;
        // 3) zero our in-bytes buffer, to start 
        // the next message
        receivedByteIdx = 0;

        // 4) zero our expected msgLen, so we'll refresh it
        // for the next command
        msgLen = 0;
    }
  }
}


bool isInTab(uint8_t value, addrTab tab){
  for (int k=0;k<tab.size;k++){
    if (tab.addrs[k]==value){
      return true;
    }
  }
  return false;

}

void setup() {
  
  // SETUP wire
  if (!Block_type){
  pinMode(RED_LED,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(RED_LED,HIGH);
  digitalWrite(LED_BUILTIN,LOW);
  }

  randomSeed(analogRead(A2));
  Wire.begin(i2c_address);
  Wire.onRequest(i2cRequestEvent);
  Wire.onReceive(i2cReceiveEvent);
  Serial.begin(9600);
  // do other setup you may need...

}
/*
 * main loop.
 * Here, we'll just loop around and handle 
 * pending commands when they come in.
 */
void loop() {
if (!scanDone){
if (cpt%50000==0 && cpt<=150000){
  Serial.println(cpt%50000);
  findAddr=I2CDevice.scan();
  while(isInTab(I2CDevice.my_addr,findAddr)){
    I2CDevice.changeAddr(findAddr);
    findAddr=I2CDevice.scan();
    Serial.println("current addr");
    Serial.println(I2CDevice.my_addr);
  };
  cpt++;
}
else if (cpt%50000!=0 && cpt<150000){
  cpt++;
}
else {
  scanDone=true;
  Serial.println("scan done");
  I2CDevice.sendReady();
}
}
  if (pendingCommandLength) {
    Serial.println("hello there");

    // oh my, we've received a command!
    
    // if you're going to be very slow in processing this,
    // you could copy the contents of pendingCommand[]
    // over to yet another buffer.
    // Here we just do it "real time" for simplicity

    // 1) process that command
    I2CDevice.process(pendingCommand, pendingCommandLength);

    // 2) zero that flag, so we don't process multiple times
    pendingCommandLength = 0;
    if (Block_type){
      I2CDevice.doThings(&val);
    }

  }

  // do anything else that needs doin'

  // main thing is that you must process any pendingCommand
  // before the next full command bytes come in through the wire

}