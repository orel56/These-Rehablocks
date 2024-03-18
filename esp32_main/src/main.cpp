#include "Controller.h"

void setup() {
  delay(5000);
  Serial.begin(9600);
  pinMode(SAP,INPUT);
  init_ble();
}

void loop() {
  int out = 0;
  out=controller_do_control();
  if(out){
    Serial.print("Code de l'erreur : ");
    Serial.println(out);
  }
  delay(200);

}