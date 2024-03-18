#include <utilities.h>

void intToBytesArray(int value, uint8_t bytes[4])
{
    Serial.print("entering intToBytesArray method, value is :");
    Serial.println(value);
    for (int i = 0; i < int(sizeof(value)); i++)
    {
        bytes[i] = (value & (0xff << ( i * 8))) >> 8*i;
        //Serial.println(bytes[i]);
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