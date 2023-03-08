#include <Arduino.h>
#include "I2Cmaster.h"
#include "Wire.h"
#include "config.h"

I2Cmaster I2Cperipheral = I2Cmaster();

void setup() {
  Serial.begin(9600);
  Wire.begin();
  attachInterrupt(SAP,I2Cperipheral.apering_process,RISING);

  }

void loop() {
  // put your main code here, to run repeatedly:
}