#include "Arduino.h"

struct subject {
    int id;
    int value;
    int old_value;
    
    subject(int id){
    this->id=id;
    this->value=0;
    this->old_value=0;
    };
};

union floatToInt {
        float    nbf;
        uint32_t nbe;
        };

typedef struct subject Subject;

void intToBytesArray(int value,uint8_t bytes[4]);
int bytesArraytoInt(volatile uint8_t* data,uint8_t len,uint8_t begin_val);

void floatToBytesArray(float value, uint8_t bytes[4]);
float bytesArraytoFloat(volatile uint8_t* data,uint8_t len,uint8_t begin_val);

void charToBytesArray(char value,uint8_t bytes[4]);
char bytesArraytoChar(volatile uint8_t* data,uint8_t len,uint8_t begin_val);

void longlongToBytesArray(int value,uint8_t bytes[8]);
