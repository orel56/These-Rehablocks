#include "access.h"

std::vector<struct Device *> value_memory;


int access (uint8_t addr);


void save_new_device(struct Device* device);