#include <Wire.h>
#include "config.h"
#include "device_factory.h"
#include <EEPROM.h>
#include <Arduino.h>

uint8_t val = 0;

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
  pinMode(SAP, OUTPUT);
  pinMode(DECO_BTN, INPUT_PULLUP);

  EEPROM.begin();
  Serial.begin(9600);
  I2CDevice->setup();
  // pas LOW plutot du analog read
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
    digitalWrite(SAP, HIGH);
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
  bool btn_val = !digitalRead(DECO_BTN);
  if (I2CDevice->mode == 0)
  {
    I2CDevice->tick();
  }
 else if (I2CDevice->mode == 1)
  {
    I2CDevice->tick();
  }
  else if (I2CDevice->mode == 2)
  {  
    if (btn_val)
    {
      I2CDevice->deconnect();
    }
    I2CDevice->tick();

  }
  else if (I2CDevice->mode == 3)
  {
    Serial.println("deco");

    val = !val;
    digitalWrite(USR_LED, val);
    delay(1000);
  }
}