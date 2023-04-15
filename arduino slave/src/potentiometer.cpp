#include "potentiometer.h"

Potentiometer::Potentiometer(int adc_pin, int threshold){
    this->adc_pin=adc_pin;
    this->threshold=threshold;
    this->id=0b10000001;
    this->linkId=0;
}

Potentiometer::Potentiometer(int adc_pin, int threshold,int id){
    this->adc_pin=adc_pin;
    this->threshold=threshold;
    this->id=(uint8_t) id;
    this->linkId=0;
}

Potentiometer::Potentiometer(int adc_pin, int threshold,int id, int linkId){
    this->adc_pin=adc_pin;
    this->threshold=threshold;
    this->id=(uint8_t) id;
    this->linkId=(uint8_t)linkId;
}

Potentiometer::Potentiometer(){

}
bool Potentiometer::get_value(){
    int current_val = 0;
    current_val=analogRead(adc_pin);
    if(check_value(current_val)){
        this->current_value=current_val;
        this->send_value=true;

    }
    else{
        this->send_value=false;
    }
    return true;

};
bool Potentiometer::check_value(int value){
    if (abs(this->current_value-value)>this->threshold){
        return true;
    }
    return false;
};

