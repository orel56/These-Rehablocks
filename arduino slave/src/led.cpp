#include "led.h"
Led::Led(int pin)
{
    this->pin = pin;
    this->id = 0b10000001;
    pinMode(pin, OUTPUT);
    this->elapsed_time = millis();

#ifdef LED_FREQUENCY
    this->frequency = LED_FREQUENCY;
    this->period = 1/LED_FREQUENCY;
#endif
#ifdef LED_TIME
    this->max_time=LED_TIME;
#endif
}

Led::Led(int pin, int id)
{
    this->pin = pin;
    this->id = id;
    pinMode(pin, OUTPUT);
    this->elapsed_time = millis();
#ifdef LED_FREQUENCY
    this->frequency = LED_FREQUENCY;
    this->period = 1/LED_FREQUENCY;
#endif
#ifdef LED_TIME
    this->max_time=LED_TIME;
#endif
}

Led::Led()
{
}

void Led::behaviour1()
{
    if (this->mouv)
    {
        this->timer = 0;
        this->elapsed_time = millis();
        this->current_value=0;
    }
    else
    {
        this->timer = (millis() - elapsed_time)/1e3;
        int blink_flag=this->timer-((unsigned long)this->max_time);
        if (blink_flag>0)
        {   
            float t = fmod(blink_flag,this->period)/ (this->period);
            this->current_value=(t<0.5) ? 1 : 0;
        }
    }
    digitalWrite(this->pin, this->current_value);
}

void Led::behaviour2()
{
    this->timer = millis()-this->elapsed_time;
    if (this->mouv)
    {
        this->timer = 0;
        this->elapsed_time = millis();
        this->current_value = 0;
    }
    else {
    this->timer = (millis() - elapsed_time)/1e3;
    int blink_flag=this->timer-((unsigned long)this->max_time);
    if (blink_flag>0 && this->current_value==0)
    {
        this->current_value = 1;
    }
    }
    digitalWrite(this->pin, this->current_value);

}

void Led::update_param(){
    if(this->pendingCommand[1]==1){
        this->frequency=bytesArraytoInt(pendingCommand,5,2);
        this->period=1/this->frequency;
    }
}

void Led::update_subject(){
    switch (pendingCommand[2])
    {
    case 1:
        this->mouv=pendingCommand[3];
        break;
    default:
        this->mouv=0;
        break;
    }
}

void Led::get_status(){
    SlaveResponse resp;
    resp.buffer[0]=this->acknowledge;
    resp.buffer[1]=0;
    resp.buffer[2]=uint8_t (this->current_value);
    resp.size=3;
    this->status=resp;
}