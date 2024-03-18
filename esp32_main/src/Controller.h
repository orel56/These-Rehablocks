#include <Bluetooth.h>
#include <i2c.h>
#include <utilities.h>

uint8_t handle_connexion(); 
uint8_t handle_deconnexion();
uint8_t get_status();
uint8_t distribute_subject();

uint8_t controller_do_control();