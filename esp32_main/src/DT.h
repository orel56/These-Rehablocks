#include "I2Cmaster.h"
#include "Xaal.h"
#include "EEPROM.h"

class DigitalTwin {
public : 
    DigitalTwin();
    void DT_get(I2Cmaster* I2Cperipheral,DeviceHandler* my_handler);
    void DT_agregate();
    void DT_analyse(DeviceHandler* my_handler);
    void DT_set(I2Cmaster* I2Cperipheral,DeviceHandler* my_handler);
    void DT_working_management();
    bool apearing_demand();
private:
    void DT_update();
};