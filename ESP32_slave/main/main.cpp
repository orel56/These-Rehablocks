#include <Wire.h>
#include "config.h"
#include "device_factory.h"
#include <EEPROM.h>
#include <Arduino.h>

uint8_t val = 0;
uint8_t sap = 0;

TwoWire I2C2 = TwoWire(1);

void i2cRequestEvent()
{
  I2CDevice->i2cRequest(&I2C2);
}

void i2cReceiveEvent(int bytesReceived)
{

  I2CDevice->i2cReceive(bytesReceived,&I2C2);
}


void setup()
{
  // SETUP wire
  delay(2000);
  pinMode(USR_LED, OUTPUT);
  pinMode(USR_BTN, INPUT_PULLUP);
  pinMode(SAP,INPUT);
  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(115200);
  I2CDevice->setup();
  byte i2c_addr;
  i2c_addr = EEPROM.read(0x00);
  Serial.print("my addr is :");
  Serial.println(i2c_addr);
  I2C2.begin(i2c_addr,SDA2,SCL2,100000UL);
  I2C2.onReceive(i2cReceiveEvent);
  I2C2.onRequest(i2cRequestEvent);

 // TWAR=(i2c_addr<<1)|1; //  enable  broadcast

  if (i2c_addr == 0x08)
  {
    I2CDevice->mode = 0;
    digitalWrite(USR_LED, HIGH);
    Serial.println("mode is 0 because i2c_addr is 8");

  }
  else
  {
    I2CDevice->mode = 2;
    digitalWrite(USR_LED, LOW);
    Serial.println("mode is 2 because i2c_addr is not 8");

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
  delay(500);
  I2CDevice->behav();
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
      I2CDevice->changeAddr(8);
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