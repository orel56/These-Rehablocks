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
        if(I2Cperipheral->ping(0x08)){
            this->new_device = 1;       
            Serial.println("There is a new device connexion tentative");
            I2Cperipheral->apering_process(my_handler);
            Serial.println("apearing done");
        }
        else {
            Serial.println("A device want to be disconnected");
            I2Cperipheral->disconnexion_process(my_handler);
            Serial.println("Disconnexion done");
        }
    }
    // Serial.println("sending the get request");
    I2Cperipheral->i2c_get_status(my_handler);
    // xaal_get();
}

void DigitalTwin::DT_agregate()
{
    
}

void DigitalTwin::DT_analyse(DeviceHandler *my_handler)
{
   
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