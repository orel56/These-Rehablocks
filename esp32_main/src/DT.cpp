#include "DT.h"

DigitalTwin::DigitalTwin(){

};

bool DigitalTwin::apearing_demand(){
    return digitalRead(SAP);
};

void DigitalTwin::DT_get(I2Cmaster * I2Cperipheral,DeviceHandler* my_handler){
    if (this->apearing_demand()){
        I2Cperipheral->apering_process(my_handler);
    }
    I2Cperipheral->i2c_get(my_handler);
    //xaal_get();
}

void DigitalTwin::DT_agregate(){

}

void DigitalTwin::DT_analyse(){

}
void DigitalTwin::DT_set(I2Cmaster * I2Cperipheral,DeviceHandler* my_handler){
    I2Cperipheral->i2c_set(my_handler);
    //xaal_tick();
}

void DigitalTwin::DT_working_management(){

}