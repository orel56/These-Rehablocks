#include <Arduino.h>

void intToBytesArray(int value, uint8_t bytes[4]);
int bytesArraytoInt(volatile uint8_t* data,uint8_t len,uint8_t begin_val);

