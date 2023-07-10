#include "I2Cmaster.h"
#include "Xaal.h"
#include "EEPROM.h"

class DigitalTwin {
public : 
    DigitalTwin();
    bool new_device=0;
    uint8_t comportement = 0;

    void DT_get(I2Cmaster* I2Cperipheral,DeviceHandler* my_handler);
    void DT_agregate(DeviceHandler *my_handler);
    void DT_analyse(DeviceHandler* my_handler);
    void DT_set(I2Cmaster* I2Cperipheral,DeviceHandler* my_handler);
    void DT_working_management();
    bool check_SAP();
    void check_device(DeviceHandler* my_handler,I2Cmaster * I2Cperipheral);
private:
    void DT_update();
};