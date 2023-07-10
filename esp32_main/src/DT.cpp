#include "DT.h"

DigitalTwin::DigitalTwin(){

};

bool DigitalTwin::check_SAP()
{
    this->new_device = 0;
    return digitalRead(SAP);
};

void DigitalTwin::DT_get(I2Cmaster *I2Cperipheral, DeviceHandler *my_handler)
{
    /*     if (my_handler->check_device_flag)
        {
            this->check_device(my_handler, I2Cperipheral);
        } */

    Serial.println(my_handler->size);

    // Serial.println("in DT get function");
    if (this->check_SAP())
    {
        if (I2Cperipheral->ping(0x08))
        {
            this->new_device = 1;
            Serial.println("There is a new device connexion tentative");
            I2Cperipheral->apering_process(my_handler);
            Serial.println("apearing done");
        }
        else
        {
            Serial.println("A device want to be disconnected");
            I2Cperipheral->disconnexion_process(my_handler);
            Serial.println("Disconnexion done");
        }
    }
    // Serial.println("sending the get request");
    I2Cperipheral->i2c_get_status(my_handler);
    // xaal_get();
}

void DigitalTwin::DT_agregate(DeviceHandler *my_handler)
{
    Subject *sub;
    Device *dev;
    std::list<uint8_t>::iterator Iter;
    uint8_t available_subjects = (my_handler->list_subject).size();
    for (uint8_t k = 0; k < available_subjects; k++)
    {
        sub = my_handler->access_subject(k, -1);
        sub->already_update = 0;
        if (!(sub->are_subscribe).empty())
        {
            if (!(sub->is_produced_by).empty())
            {
                for (Iter = (sub->is_produced_by).begin(); Iter != (sub->is_produced_by).end(); Iter++)
                {
                    dev = my_handler->access_dev(*Iter);
                    switch (sub->id)
                    {
                    case 128:
                    {
                        bool val = (abs(dev->prev_x - dev->x) > 10 || abs(dev->prev_y - dev->y) > 10);
                        if (sub->already_update)
                        {
                            Serial.println("this subject has already been updated but we can verify using this other device if the value is ok");
                        }
                        else
                        {
                            sub->prev_value = sub->value;
                            sub->value = val;
                        }
                    }
                        break;
                    case 64:
{                        sub->value = 0;
}                        break;
                    case 32:
{                        sub->value = 0;
}                        break;
                    case 16:
{                        sub->value = 0;
}                        break;
                    case 8:
                       { bool val = dev->current_value;

                        if (sub->already_update)
                        {
                            Serial.println("this subject has already been updated but we can verify using this other device if the value is ok");
                        }
                        else
                        {
                            sub->prev_value = sub->value;
                            sub->value = val;
                        }}
                        break;
                    case 4:
                        {bool val = (abs(dev->previous_value - dev->current_value) > 100);
                        if (sub->already_update)
                        {
                            Serial.println("this subject has already been updated but we can verify using this other device if the value is ok");
                        }
                        else
                        {
                            sub->prev_value = sub->value;
                            sub->value = val;
                        }}
                        break;
                    case 2:
                        {bool val = dev->current_value;
                        if (sub->already_update)
                        {
                            Serial.println("this subject has already been updated but we can verify using this other device if the value is ok");
                        }
                        else
                        {
                            sub->prev_value = sub->value;
                            sub->value = val;
                        }}
                        break;
                    default:
                        break;
                    }
                    if (!sub->already_update)
                    {
                        sub->already_update = 1;
                    }
                }
            }
            else if (!(sub->substitute).empty())
            {
                Serial.println("we need to evaluate subsitutes of this subject in order to produce it");

            }
        }
    }
}

void DigitalTwin::DT_analyse(DeviceHandler *my_handler)
{   Subject *sub;
    uint8_t available_subjects = (my_handler->list_subject).size();
    for (uint8_t k = 0; k < available_subjects; k++)
    {
        sub = my_handler->access_subject(k, -1);
        if (sub->value != sub->prev_value){
            sub->to_be_sent=1;
        }
        else {
            sub->to_be_sent=0;
        }
    }

}

void DigitalTwin::DT_set(I2Cmaster *I2Cperipheral, DeviceHandler *my_handler)
{
}

void DigitalTwin::DT_working_management()
{
}

/* void DigitalTwin::check_device(DeviceHandler *my_handler, I2Cmaster *I2Cperipheral)
{
    int error = 0;
    Serial.println("device was in quarantine is it ok ? ");
    Device *tmp = NULL;
    for (int i = 0; i < my_handler->size; i++)
    {
        tmp = access(&(my_handler->list_dev), 0x09 + i);
        if ((tmp)->quarantine)
        {
            error = I2Cperipheral->ping((tmp)->addr);
            if (error)
            {
                Serial.println("error when pinging device, deletion begin ");

                my_handler->delete_device((tmp)->addr);
            }
            else
            {
                tmp->quarantine = 0;
            }
        }
    }
    my_handler->check_device_flag = 0;
} */