#include <config.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <string.h>
#include <Device.h>

int init_ble();
void check_ble(Device * nodes[10], uint8_t n);
std::string data_to_send(Device * nodes[10], uint8_t n);