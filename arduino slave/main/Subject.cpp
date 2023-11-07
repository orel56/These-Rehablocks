#include "Subject.h"


Subject * new_subject(int id){
    Subject * sub = new Subject();
    sub->id=id;
    sub->value=0;
    sub->old_value=0;
    return sub;
}



uint8_t *intToBytesArray(int value)
{
    static unsigned char bytes[sizeof(value)];
    for (unsigned int i = 0; i < sizeof(value); i++)
    {
        bytes[i] = (value & (0x000000ff << (2 * i * 4))) >> 8 * i;
    }
    return bytes;
}

int bytesArraytoInt(volatile uint8_t *data, uint8_t len, uint8_t begin_val)
{
    int value = 0;
    int buffer = 0;
    for (int i = begin_val; i < len; i++)
    {
        buffer = data[i];
        buffer = buffer << ((i - begin_val) * 8);
        value += buffer;
    }
    Serial.println("extracted value from data received : ");
    Serial.println(value);

    return value;
}

uint8_t *floatToBytesArray(float value)
{
    floatToInt converter;
    converter.nbf =value;
    uint32_t y = converter.nbe;
    unsigned char *my_value_in_bytes;

    my_value_in_bytes = intToBytesArray(y);
    return my_value_in_bytes;
}
float bytesArraytoFloat(volatile uint8_t *data, uint8_t len, uint8_t begin_val)
{
    int z = 0;
    z = bytesArraytoInt(data, len, begin_val);
    floatToInt converter;
    converter.nbe =z;
    float w = converter.nbf;
    return w;
}

uint8_t *charToBytesArray(char value)
{
    int y = value;
    unsigned char *my_value_in_bytes;

    my_value_in_bytes = intToBytesArray(y);
    return my_value_in_bytes;
}

char bytesArraytoChar(volatile uint8_t *data, uint8_t len, uint8_t begin_val)
{
    int z = 0;
    z = bytesArraytoInt(data, len, begin_val);
    return (char)z;
}

void swapValues(Subject * my_subject,uint8_t* new_value){
    my_subject->old_value = my_subject->value;
    

}