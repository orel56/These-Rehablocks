#include <device.h>
#include <cstring>
#include "database.h"
#include <config.h>
#include "Arduino.h"
#include <vector>

struct SlaveResponse {
    uint8_t buffer[20]={};
    uint8_t size=0;
};


class DeviceHandler {
public:
  bool check_device_flag=0;
  std::list<Device*> list_device;
  std::list<Subject*> list_subject;

  Device* filter_buffer[MAX_DEVICE];
  uint8_t size_buffer;
  uint8_t size;
  uint8_t next_addr; 

  DeviceHandler();
  //DeviceHandler(list_device * list_dev, uint8_t size);
  ~DeviceHandler();

  void add_new_device(uint8_t addr,uint8_t * buffer);
  void update_value(uint8_t addr, SlaveResponse resp);
  void delete_device(uint8_t addr);
  void filter_list (const char* filter);
  Device* access_dev(uint8_t addr,int8_t position=-1);

  void update_subjects();
  Subject* access_subject(int8_t position=-1,int8_t id = -1);
  void update_subjects_value();
  void add_new_subject(uint8_t id);

  uint8_t ask_free_addr();
  void put_in_quarantine(uint8_t addr);

};