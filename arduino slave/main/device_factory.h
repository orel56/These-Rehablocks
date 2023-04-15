#ifdef LED  
    #include "led.h"
    #ifdef LED_ID 
        #ifdef Led_LINKID
            Led* I2CDevice = new Led(LED_PIN,LED_ID, LED_LINKID);
        #else
            Led* I2CDevice = new Led(LED_PIN,LED_ID);
        #endif
    #else
        Led* I2CDevice = new Led(LED_PIN);

    #endif
#elif POTENTIOMETER 
    #include "potentiometer.h"
    #ifdef POTENTIOMETER_ID 
        #ifdef POTENTIOMETER_LINKID
            Potentiometer * I2CDevice = new Potentiometer(POTENTIOMETER_PIN,POTENTIOMETER_THRESHOLD,POTENTIOMETER_ID,POTENTIOMETER_LINKID);
        #else
            Potentiometer * I2CDevice = new Potentiometer(POTENTIOMETER_PIN,POTENTIOMETER_THRESHOLD,POTENTIOMETER_ID);
        #endif
    #else
        Potentiometer * I2CDevice = new Potentiometer(POTENTIOMETER_PIN,POTENTIOMETER_THRESHOLD);

    #endif
#endif