#include "DT.h"

DigitalTwin::DigitalTwin(){

};

void DigitalTwin::DT_get(I2Cmaster * I2Cperipheral){
    I2Cperipheral->i2c_get();
    xaal_get();
}

void DigitalTwin::DT_agregate(){

}

void DigitalTwin::DT_analyse(){

}
void DigitalTwin::DT_set(I2Cmaster * I2Cperipheral){
    I2Cperipheral->i2c_set();
    xaal_set();
}

void DigitalTwin::DT_working_management(){

}