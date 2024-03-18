#include "led.h"
Led::Led(int pin)
{
    this->pin = pin;
    this->id = 0b10000001;
    pinMode(pin, OUTPUT);
    this->elapsed_time = millis();

#ifdef LED_FREQUENCY
    this->frequency = LED_FREQUENCY;
    this->period = 1 / LED_FREQUENCY;
#endif
#ifdef LED_TIME
    this->max_time = LED_TIME;
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
    this->period = 1 / LED_FREQUENCY;
#endif
#ifdef LED_TIME
    this->max_time = LED_TIME;
#endif
}

void Led::init_produced_subject()
{
    this->produced_subject_nbr = 2;
    this->producedSubjects[0] = new Subject(0b00000001);
    this->producedSubjects[1] = new Subject(0b00001000);
}

Led::Led()
{
}

void Led::behaviour1()
{
    bool mouv = bool(this->producedSubjects[1]->value);
    if (mouv)
    {
        this->timer = 0;
        this->elapsed_time = millis();
        this->current_value = 0;
    }
    else
    {
        this->timer = (millis() - elapsed_time) / 1e3;
        int blink_flag = this->timer - ((unsigned long)this->max_time);
        if (blink_flag > 0)
        {
            float t = fmod(blink_flag, this->period) / (this->period);
            this->current_value = (t < 0.5) ? 1 : 0;
        }
    }
    digitalWrite(this->pin, this->current_value);
}

void Led::behaviour2()
{
    bool mouv = bool(this->producedSubjects[1]->value);
    this->timer = millis() - this->elapsed_time;
    if (mouv)
    {
        this->timer = 0;
        this->elapsed_time = millis();
        this->current_value = 0;
    }
    else
    {
        this->timer = (millis() - elapsed_time) / 1e3;
        int blink_flag = this->timer - ((unsigned long)this->max_time);
        if (blink_flag > 0 && this->current_value == 0)
        {
            this->current_value = 1;
        }
    }
    digitalWrite(this->pin, this->current_value);
}

void Led::update_param()
{
    if (this->pendingCommand[1] == 1)
    {
        this->frequency = bytesArraytoInt(pendingCommand, 5, 2);
        this->period = 1 / this->frequency;
    }
}

void Led::produce_subjects()
{
    for (int id = 0; id < this->produced_subject_nbr; id++)
    {
        if ((this->producedSubjects[id])->id == 0b00001000)
        {
            (this->producedSubjects[id])->old_value = this->previous_value;
            (this->producedSubjects[id])->value = this->current_value;
        }
        else
        {
            (this->producedSubjects[id])->old_value = 1;
            (this->producedSubjects[id])->value = 1;
        }
    }
}
