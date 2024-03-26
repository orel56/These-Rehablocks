#include "Subject.h"

void intToBytesArray(int value, uint8_t bytes[4])
{
    
    for (int i = 0; i < 4; i++)
    {
        bytes[i] = (value & (0xff << ( i * 8))) >> 8*i;
    } 

}

void longlongToBytesArray(int value, uint8_t bytes[8])
{
    
    for (int i = 0; i < 8; i++)
    {
        bytes[i] = (value & (0xff << ( i * 8))) >> 8*i;
    } 

}

int bytesArraytoInt(volatile uint8_t *data, uint8_t len, uint8_t begin_val)
{
    int value = 0;
    int buffer = 0;
    for (int i = begin_val; i < (len + begin_val); i++)
    {
        buffer = data[i];
        buffer = buffer << ((i - begin_val) * 8);
        value += buffer;
    }
    return value;
}

void floatToBytesArray(float value,uint8_t bytes[4])
{
    floatToInt converter;
    converter.nbf = value;
    uint32_t y = converter.nbe;
    intToBytesArray(y,bytes);
}
float bytesArraytoFloat(volatile uint8_t *data, uint8_t len, uint8_t begin_val)
{
    int z = 0;
    z = bytesArraytoInt(data, len, begin_val);
    floatToInt converter;
    converter.nbe = z;
    float w = converter.nbf;
    return w;
}

void charToBytesArray(char value, uint8_t bytes[4])
{
    int y = value;
    intToBytesArray(y,bytes);
}

char bytesArraytoChar(volatile uint8_t *data, uint8_t len, uint8_t begin_val)
{
    int z = 0;
    z = bytesArraytoInt(data, len, begin_val);
    return (char)z;
}

void swapValues(Subject *my_subject, uint8_t *new_value)
{
    my_subject->old_value = my_subject->value;
}