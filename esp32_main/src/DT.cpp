#include "DT.h"

DigitalTwin::DigitalTwin(){

};

bool DigitalTwin::apearing_demand(){
    return digitalRead(SAP);
};

void DigitalTwin::DT_get(I2Cmaster * I2Cperipheral,DeviceHandler* my_handler){
    if(this->check_device_flag){
        this->check_device(my_handler,I2Cperipheral);
    }
    if (this->apearing_demand()){
        I2Cperipheral->apering_process(my_handler);
    }
    I2Cperipheral->i2c_get(my_handler);
    //xaal_get();
}

void DigitalTwin::DT_agregate(){

}

void DigitalTwin::DT_analyse(DeviceHandler* my_handler){
    for (int i = 0; i<my_handler->size;i++){
        (my_handler->list_device[i])->analyse();
    }
}

void DigitalTwin::DT_set(I2Cmaster * I2Cperipheral,DeviceHandler* my_handler){
    I2Cperipheral->i2c_set(my_handler);
    //xaal_tick();
}

void DigitalTwin::DT_working_management(){

}

bool DigitalTwin::check_device(DeviceHandler* my_handler,I2Cmaster * I2Cperipheral){
    int error=0;
    for (int i = 0; i<my_handler->size;i++){
        if ((my_handler->list_device[i])->quarantine){
            error=I2Cperipheral->ping((my_handler->list_device[i])->addr);
            if (error){
                my_handler->delete_device((my_handler->list_device[i])->addr);
            }

        }
    }

}