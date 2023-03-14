#include "actuator.h"

Actuator::Actuator(){};

void Actuator::process(volatile uint8_t * buffer, uint8_t len){
    Device::process(buffer,len);
    

}
