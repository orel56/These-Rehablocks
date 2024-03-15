#include "joystick.h"

joystick::joystick()
{
}

void joystick::init_produced_subject()
{
    this->produced_subject_nbr = 2;
    this->producedSubjects[0] = new Subject(0);
    this->producedSubjects[1] = new Subject(4);
}

joystick::joystick(int pinx, int piny, int threshold, int id)
{
    this->pinx = pinx;
    this->piny = piny;
    this->id = id;
    this->threshold = threshold;
    pinMode(piny, INPUT);
    pinMode(pinx, INPUT);

    this->init_produced_subject();
}

void joystick::behaviour1()
{

    if (this->firsttime)
    {
        this->drift[0] = analogRead(pinx);
        this->drift[1] = analogRead(piny);
        this->firsttime = 0;
    }

    int readX = analogRead(pinx) - this->drift[0];
    int readY = analogRead(piny) - this->drift[1];

    if (abs(this->valueX[0] - readX) > this->threshold)
    {
        this->valueX[1] = this->valueX[0];
        this->valueX[0] = readX;
    }

    if (abs(this->valueY[0] - readY) > this->threshold)
    {
        this->valueY[1] = this->valueY[0];
        this->valueY[0] = readY;
    }
}

void joystick::update_param()
{
    if (this->pendingCommand[1] == 1)
    {
        this->threshold = bytesArraytoInt(pendingCommand, 5, 2);
    }
}

void joystick::produce_subjects()
{
    for (int id = 0; id < this->produced_subject_nbr; id++)
    {
        if ((this->producedSubjects[id])->id == 4)
        {
            
            (this->producedSubjects[id])->old_value = (this->producedSubjects[id])->value;

            (this->producedSubjects[id])->value = this->get_direction();

        }
        else
        {
            (this->producedSubjects[id])->old_value = 1;
            (this->producedSubjects[id])->value = 1;
        }
    }
}
uint8_t joystick::get_direction()
{
    uint8_t direction = 0; // 0= pas de changement de direction, 1= est, 2=Nord-est, 3= nord,....

    if (this->valueX[0] < -20)
    {
        if (this->valueY[0] < -20)
        {
            direction = 6;
        }
        else if (this->valueY[0] > 20)
        {
            direction = 4;
        }
        else
        {
            direction = 5;
        }
    }
    else if (this->valueX[0] > 20)
    {
        if (this->valueY[0] < -20)
        {
            direction = 8;
        }
        else if (this->valueY[0] > 20)
        {
            direction = 2;
        }
        else
        {
            direction = 1;
        }
    }
    else
    {
        if (this->valueY[0] < -20)
        {
            direction = 7;
        }
        else if (this->valueY[0] > 20)
        {
            direction = 3;
        }
        else
        {
            direction = 0;
        }
    }
    Serial.println(direction);

    return direction;
}
