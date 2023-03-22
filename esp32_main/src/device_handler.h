#include <ArduinoJson.h>
#include <vector>
struct SlaveResponse {
    uint8_t buffer[20]={};
    uint8_t size=0;

};

void add_new_device(SlaveResponse* response_info, uint8_t addr);

uint8_t ask_free_addr(); 

std::vector<uint8_t[2]> filter_list (const char* filter);