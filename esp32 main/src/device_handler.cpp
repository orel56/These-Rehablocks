#include "device.h"
#include "utils.cpp"

DeviceList list_devices=DeviceList();


void add_new_device(SlaveResponse* response_info){};

uint8_t ask_free_addr(uint8_t addr){
    //return list_devices.get_free_addr();
    return addr + 1;
}; 