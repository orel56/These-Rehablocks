#include "I2Cmaster.h"
#include "Xaal.h"
#include "EEPROM.h"

class DigitalTwin {
public : 
    DigitalTwin();
    void DT_get(I2Cmaster* I2Cperipheral);
    void DT_agregate();
    void DT_analyse();
    void DT_set(I2Cmaster* I2Cperipheral);
    void DT_working_management();

private:
    void DT_update();
};