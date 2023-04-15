#include "led.h"

Led::Led(int pin){
    this->pin=pin;
    this->id==0b10000001;
    this->linkId=0;

    pinMode(pin,OUTPUT);
}

Led::Led(int pin, int id){
    this->pin=pin;
    this->id=id;
    this->linkId=0;

    pinMode(pin,OUTPUT);
}

Led::Led(int pin,int id, int linkId){
    this->pin=pin;
    this->id=id;
    this->linkId=linkId;
    pinMode(pin,OUTPUT);
}

Led::Led(){

}

bool Led::is_setable(int value){
    Serial.println("checking if is setable");
    if (this->current_value!=value){
        this->current_value=value;
        is_set=true;
        return true;
    }
    else {
        is_set=false;
        return false;
    }
}
bool Led::set_value(int value){
    Serial.println("setting led to value : ");
    Serial.println(value);


    if (is_setable(value)){
        digitalWrite(this->pin,value);
    }
    return false;
}