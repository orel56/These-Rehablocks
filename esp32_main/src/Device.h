#include <Arduino.h>

struct subject {
    uint8_t id; 
    int value;
};

typedef struct subject Subject;

struct devices {
    uint8_t addr;
    int status;
    int id;
    int type;
    int subscription;
    Subject *subjects[20];
    int sub_number = 0;
};

typedef struct devices Device;

