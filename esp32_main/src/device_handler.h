#include <device.h>
#include <cstring>
#include <vector>
#include <config.h>
#include "Arduino.h"

struct SlaveResponse {
    uint8_t buffer[20]={};
    uint8_t size=0;

};


class DeviceHandler {
public:
  bool check_device_flag=0;

  list_device *list_dev=NULL;
  Device* filter_buffer[MAX_DEVICE];
  uint8_t size_buffer;
  uint8_t size;
  DeviceHandler();
  //DeviceHandler(list_device * list_dev, uint8_t size);
  ~DeviceHandler();
  void add_new_device(uint8_t addr,uint8_t id, uint8_t subscription,uint8_t behaviour);
  uint8_t ask_free_addr();
  void update_value(uint8_t addr, int value = 0, bool previous=0);
  void put_in_quarantine(uint8_t addr);
  void delete_device(uint8_t addr);
  void filter_list (const char* filter);

};