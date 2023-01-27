/* MyI2CPeripheral
 * A class to centralize our peripheral's state 
 * and message processing.
 */

#include "MyI2Cdevice.h"
#include "config.h"

char*mycommands[6]={strdup("connexion"),strdup("change_addr"),strdup("get_value"),strdup("green_led"),strdup("red_led"),strdup("get_type")};

uint16_t hex_val;

SlaveResponse MyI2CPeripheral::getResponse(int val){
    SlaveResponse response;
    Serial.print("command in get response : ");
    Serial.println(command);
    if (strcmp(command,"get_value")==0){
       uint8_t value_octet[2];
       value_octet[0]=(uint8_t) (val & 0xff) ;
       value_octet[1]=(uint8_t)( (val >> 8));
       response.buffer[0]= value_octet[0];
       response.buffer[1]= value_octet[1];
       Serial.println(val);
       response.size=2;
       return response;
    }
    else if (strcmp(command,"get_type")==0){

      response.buffer[0]=(uint8_t) Block_type;
      response.size =1;
      return response;

    }
    else {
       response.buffer[0]= 0x01;

       response.size=1;
       return response;
    }
  
};

uint8_t MyI2CPeripheral::expectedReceiveLength(uint8_t forRegister){
  if (forRegister!=0x01){
    return 1;
  }
  else {return 2;}
};

void MyI2CPeripheral::process(volatile uint8_t * buffer, uint8_t len){
    command=mycommands[buffer[0]];
    Serial.println(command);
   if (strcmp(command,"change_addr")==0){
      TWAR = buffer[1] << 1;
    }
   else if (strcmp(command,"green_led")==0){
    digitalWrite(RED_LED,LOW);
    digitalWrite(LED_BUILTIN,HIGH);
   }
   else if (strcmp(command,"red_led")==0){
    digitalWrite(RED_LED,HIGH);
    digitalWrite(LED_BUILTIN,LOW);
   }
    
};

void MyI2CPeripheral::doThings(int* val){
    *val = analogRead(A0);
};

MyI2CPeripheral::MyI2CPeripheral(){};


