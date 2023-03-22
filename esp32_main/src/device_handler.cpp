#include "device_handler.h"

std::vector<uint8_t> list_devices;
std::vector<uint8_t> addrs;

void add_new_device(SlaveResponse* response_info, uint8_t addr){
    for (int i=0;i<response_info->size;i++){
        addrs.push_back(addr);
        list_devices.push_back(response_info->buffer[i]);
    }
};

uint8_t ask_free_addr(){
    if (addrs.empty()){
        return 0x09;
    }
    return addrs.back()+1;
}; 