#include "Arduino.h"

struct subject {
    int id;
    int value;
    int old_value;
};

union floatToInt {
        float    nbf;
        uint32_t nbe;
        };

typedef struct subject Subject;

Subject * new_subject(int id);

uint8_t * intToBytesArray(int value);
int bytesArraytoInt(volatile uint8_t* data,uint8_t len,uint8_t begin_val);

uint8_t * floatToBytesArray(float value);
float bytesArraytoFloat(volatile uint8_t* data,uint8_t len,uint8_t begin_val);

uint8_t * charToBytesArray(char value);
char bytesArraytoChar(volatile uint8_t* data,uint8_t len,uint8_t begin_val);
