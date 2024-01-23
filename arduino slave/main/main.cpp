#include <Wire.h>
#include "config.h"
#include "device_factory.h"
#include <EEPROM.h>
#include <Arduino.h>

uint8_t val = 0;
uint8_t sap = 0;

void i2cRequestEvent()
{
  I2CDevice->i2cRequest();
}

void i2cReceiveEvent(int bytesReceived)
{

  I2CDevice->i2cReceive(bytesReceived);
}

void setup()
{
  // SETUP wire
  pinMode(USR_LED, OUTPUT);
  pinMode(USR_BTN, INPUT_PULLUP);
  pinMode(SAP,INPUT);
  EEPROM.begin();
  Serial.begin(9600);
  I2CDevice->setup();
  byte i2c_addr;
  i2c_addr = EEPROM.read(0x00);
  Wire.begin(i2c_addr);
  TWAR=(i2c_addr<<1)|1; //  enable  broadcast
  Wire.onRequest(i2cRequestEvent);
  Wire.onReceive(i2cReceiveEvent);
  if (i2c_addr == 0x08)
  {
    I2CDevice->mode = 0;
    digitalWrite(USR_LED, HIGH);

  }
  else
  {
    I2CDevice->mode = 2;
    digitalWrite(USR_LED, LOW);

  }
}

// do other setup you may need..
/*
 * main loop.
 * Here, we'll just loop around and handle
 * pending commands when they come in.
 */
void loop()
{
  delay(1000);
  
  bool btn_val = !digitalRead(USR_BTN);

  if (I2CDevice->mode == 0)
  {  
    if(!sap){ // if sap is not required yet
      Serial.println("Waiting for button ");
      sap=btn_val; //check the btn value and store it in sap
      if (sap){ // if there was a change on sap (ie btn was pressed ) put SAP to OUTPUT and HIGH
        pinMode(SAP,OUTPUT);
        digitalWrite(SAP,HIGH);
      }
    }
  }
  else if (I2CDevice->mode == 2)
  {  
    if (btn_val)
    {
      I2CDevice->deconnect();
    }
  }
  else if (I2CDevice->mode == 3)
  {
    Serial.println("deco");
    val = !val;
    digitalWrite(USR_LED, val);
  }
}