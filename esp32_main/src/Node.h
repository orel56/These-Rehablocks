#include <Arduino.h>

struct subject {
    uint8_t id; 
    int value;
};

typedef struct subject Subject;

struct nodes {
    uint8_t addr;
    int status;
    int id;
    int type;
    uint8_t behav = 0;
    long long subscription;
    long long production;
    Subject *produced_subjects[10];
    Subject *subscribed_subjects[10];
    uint8_t prod_sub_number = 0;
    uint8_t subs_sub_number = 0;

};

typedef struct nodes Node;

