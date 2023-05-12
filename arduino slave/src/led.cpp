#include "led.h"
Led::Led(int pin)
{
    this->pin = pin;
    this->id == 0b10000001;
    int i = 0;
    pinMode(pin, OUTPUT);
    this->elapsed_time = millis()

#ifdef LED_FREQUENCY
                             this->frequency = LED_FREQUENCY;
#endif
}

Led::Led(int pin, int id)
{
    this->pin = pin;
    this->id = id;
    int i = 0;
    pinMode(pin, OUTPUT);
    this->elapsed_time = millis();
#ifdef LED_FREQUENCY
    this->frequency = LED_FREQUENCY;
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
    }
    else
    {
        this->timer = millis() - elapsed_time;
        if (this->timer > ((unsigned long)this->max_time * 60000))
        {
            if (this->t >= 0.5)
            {
            }
            else 
            {
                this->t = (this->timer-((unsigned long)this->max_time * 60000))/ (period * 1000);
                this->current_value = !(this->current_value);
                digitalWrite(this->pin, this->current_value);


            }
        }
    }
}

void Led::behaviour2()
{
    this->timer = millis();
    if (this->mouv)
    {
        this->timer = 0;
    }
    if (this->timer == this->maxtime)
    {
        this->current_value = 1;
        digitalWrite(this->pin, this->current_value);
    }
}
