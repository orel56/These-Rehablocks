#include "DT.h"
DigitalTwin * mytwin = new DigitalTwin();
I2Cmaster * I2Cperipheral = new I2Cmaster();
DeviceHandler* my_handler=new DeviceHandler();

void setup() {
  delay(5000);
  Serial.begin(9600);
  pinMode(SAP,INPUT);
  I2Cperipheral->i2c_init(my_handler);
  //xaal_init();
}

void loop() {
  mytwin->DT_get(I2Cperipheral,my_handler);
  //mytwin->DT_agregate();
  //mytwin->DT_analyse(my_handler);
  //mytwin->DT_set(I2Cperipheral,my_handler);
  //mytwin->DT_working_management();
  //DT_get --> réccupère les infos de xaal et de i2c
  //DT_aggregate --> aggrège les données réccupéerée précédemment et les anciennes données 
  //DT_analysis --> analyse des données agrégées 
  //DT_set --> on envoie les infos à xaal et sur i2c
  //DT_working_management --> défini un nouveaux mode de fonctionnement 
  delay(1000);

}