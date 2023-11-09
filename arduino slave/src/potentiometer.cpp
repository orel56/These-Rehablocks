#include "potentiometer.h"

Potentiometer::Potentiometer(){

}

void Potentiometer::init_produced_subject(){
    this->produced_subject_nbr=2;
    this->producedSubjects[0]= new Subject(0b00000001);
    this->producedSubjects[1]= new Subject(0b00000100);
}

Potentiometer::Potentiometer(int adc_pin, int threshold, int id){
    this->adc_pin = adc_pin;
    this->id = id;
    this->threshold=threshold;
    pinMode(adc_pin, INPUT);
    this->init_produced_subject();
}

void Potentiometer::behaviour1(){
    int read =analogRead(adc_pin);
    Serial.println(abs(this->current_value - read));
    if(abs(this->current_value - read)>this->threshold){
        this->previous_value=this->current_value;
        this->current_value=read;
    }
}

void Potentiometer::update_param(){
     if(this->pendingCommand[1]==1){
        this->threshold=bytesArraytoInt(pendingCommand,5,2);
    }
}

void Potentiometer::produce_subjects(){
    for(int id =0; id<this->produced_subject_nbr;id++){
        if ((this->producedSubjects[id])->id==0b00000100){

            (this->producedSubjects[id])->old_value=this->previous_value;

            (this->producedSubjects[id])->value=this->current_value;

        }
        else {
            (this->producedSubjects[id])->old_value = 1;
            (this->producedSubjects[id])->value=1;
        }
    }
}
