#ifdef LED  
    #include "led.h"
    #ifdef LED_ID 
        Led* I2CDevice = new Led(LED_PIN,LED_ID);
    #else
        Led* I2CDevice = new Led(LED_PIN);

    #endif
#elif POTENTIOMETER 
    #include "potentiometer.h"
    #ifdef POTENTIOMETER_ID 
        Potentiometer * I2CDevice = new Potentiometer(POTENTIOMETER_PIN,POTENTIOMETER_THRESHOLD,POTENTIOMETER_ID);
    #else
        Potentiometer * I2CDevice = new Potentiometer(POTENTIOMETER_PIN,POTENTIOMETER_THRESHOLD);

    #endif
#endif
