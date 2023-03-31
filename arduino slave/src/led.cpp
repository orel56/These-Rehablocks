#include "led.h"

Led::Led(int pin){
    this->pin=pin;
    pinMode(pin,OUTPUT);
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