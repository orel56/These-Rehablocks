#include "Device.h"

Device * I2CDevice = nullptr;

#ifdef LED  
    #include "devices/led.h"
    I2CDevice = new Led();
#elif POTENTIOMETER 
    #include "devices/potentiometer.h"
    I2CDevice = new Potentiometer();
#endif
