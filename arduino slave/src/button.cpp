#include "button.h"

Button::Button(){

}

void Button::init_produced_subject(){
    this->produced_subject_nbr=2;
    this->producedSubjects[0]= new Subject(0);
    this->producedSubjects[1]= new Subject(17);
}

Button::Button(int pin, int id){
    this->pin = pin;
    this->id = id;
    pinMode(pin, INPUT);
    this->init_produced_subject();
}

void Button::behaviour1(){
    int read =analogRead(pin);
    this->previous_value=this->current_value;
    this->current_value=read;}

void Button::produce_subjects(){
    for(int id =0; id<this->produced_subject_nbr;id++){
        if ((this->producedSubjects[id])->id==16){

            (this->producedSubjects[id])->old_value=this->previous_value;

            (this->producedSubjects[id])->value=this->current_value;
            
        }
        else {
            (this->producedSubjects[id])->old_value = 1;
            (this->producedSubjects[id])->value=1;
        }
    }
}
