#ifdef LED  
    #include "led.h"
    Led* I2CDevice = new Led(LED_PIN);
#elif POTENTIOMETER 
    #include "potentiometer.h"
    Potentiometer * I2CDevice = new Potentiometer(POTENTIOMETER_PIN,POTENTIOMETER_THRESHOLD);
#endif