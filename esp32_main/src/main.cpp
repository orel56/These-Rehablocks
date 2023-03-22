#include "I2Cmaster.h"
#include "Xaal.h"
#include "DT.h"
DigitalTwin * mytwin = new DigitalTwin();
I2Cmaster * I2Cperipheral = new I2Cmaster();

void apaering_process(){
  I2Cperipheral->apering_process();
}

void setup() {
  Serial.begin(9600);
  I2Cperipheral->i2c_init();
  xaal_init();
  attachInterrupt(SAP,apaering_process,RISING);
}

void loop() {
  mytwin->DT_get(I2Cperipheral);
  mytwin->DT_agregate();
  mytwin->DT_analyse();
  mytwin->DT_set(I2Cperipheral);
  mytwin->DT_working_management();
  //DT_get --> réccupère les infos de xaal et de i2c
  //DT_aggregate --> aggrège les données réccupéerée précédemment et les anciennes données 
  //DT_analysis --> analyse des données agrégées 
  //DT_set --> on envoie les infos à xaal et sur i2c
  //DT_working_management --> défini un nouveaux mode de fonctionnement 

}