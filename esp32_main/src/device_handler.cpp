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

std::vector<uint8_t[2]> filter_list (const char* filter){
    std::vector<uint8_t[2]> device_filtered;
    uint8_t x[2];
    if (!(strcmp(filter,"sensor"))){
        for (int i=0;i<list_devices.size();i++){
            if (list_devices[i]==0){
                x[0]=i+9;
                x[1]=0;
                device_filtered.push_back(x);
            }
        }
    }
    else if (!(strcmp(filter,"actuator"))){
        for (int i=0;i<list_devices.size();i++){
            if (list_devices[i]==1){
                x[0]=i+9;
                x[1]=1;
                device_filtered.push_back(x);
            }
        }
    }
    return device_filtered;
         
}