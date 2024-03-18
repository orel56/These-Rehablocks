#include "ultrason.h"

ultrason::ultrason()
{
}

void ultrason::init_produced_subject()
{
    this->produced_subject_nbr = 2;
    this->producedSubjects[0] = new Subject(0);
    this->producedSubjects[1] = new Subject(35);
}

ultrason::ultrason(int pinecho, int pintrig, int threshold, int id)
{
    this->pinecho = pinecho;
    this->pintrig = pintrig;
    this->id = id;
    this->threshold = threshold;

    pinMode(pintrig, OUTPUT);
    pinMode(pinecho, INPUT);

    this->init_produced_subject();
}

void ultrason::behaviour1()
{
  float duration;

  digitalWrite(this->pintrig, LOW);
  delayMicroseconds(2);
  digitalWrite(this->pintrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(this->pintrig, LOW);

  
  duration = pulseIn(this->pinecho, HIGH);
  distance[1] = distance[0];

    Serial.print("durée en us : ");
    Serial.println(duration);

  distance[0] = ((duration*.0343)/2)*10;

  Serial.print("Distance in mm: ");
  Serial.println(distance[0]);
}

void ultrason::update_param()
{
    if (this->pendingCommand[1] == 1)
    {
        this->threshold = bytesArraytoInt(pendingCommand, 5, 2);
    }
}

void ultrason::produce_subjects()
{
    for (int id = 0; id < this->produced_subject_nbr; id++)
    {
        if ((this->producedSubjects[id])->id == 35)
        {
            
            (this->producedSubjects[id])->old_value = (this->producedSubjects[id])->value;

            (this->producedSubjects[id])->value = this->get_distance();

        }
        else
        {
            (this->producedSubjects[id])->old_value = 1;
            (this->producedSubjects[id])->value = 1;
        }
    }
}

uint8_t ultrason::get_distance(){
    if (this->distance[0]>this->threshold){
        return 0;
    }
    else {
        return 1;
    }

}
