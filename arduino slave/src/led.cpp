#include "led.h"

Led::Led(int pin){
    this->pin=pin;
}


bool Led::is_setable(int value){
    if (this->current_value!=value){
        is_set=true;
        return true;
    }
    else {
        is_set=false;
        return false;
    }
}
bool Led::set_value(int value){

    if (is_setable(value)){
        digitalWrite(this->pin,value);
    }
    return false;
}