#include <utilities.h>

volatile uint8_t receivedBytes[RECEIVED_COMMAND_MAX_BYTES];

int send_command(uint8_t addr,const char * command, uint8_t value = 0) {
    uint8_t * datas;
    uint8_t bytes;
if (command=="ping"){
    datas[0]=0x00;
    bytes=1;
}
else if (command=="change_addr"){
    datas[0]=0x01;
    datas[1]=value;
    bytes=2;
}
else if (command=="get_info"){
    datas[0]=0x02;
    bytes=1;
}
  Wire.beginTransmission(addr);
  //Serial.println(datas[0]);
  for (int i=0; i<bytes;i++){
    //Serial.print(datas[i]);
    Wire.write(datas[i]);
  }
  int out=Wire.endTransmission();
  return out;
}

void receive_data(uint8_t addr,uint8_t * data_buffer, uint8_t bytesToBeReceived){
    const uint8_t bytesReceived=Wire.requestFrom(addr,bytesToBeReceived);
    for (int i=0; i<bytesReceived;i++){
        uint8_t dataRead=Wire.read();
        data_buffer[i]=dataRead;
    }
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
    for (int i = begin_val; i < (len+begin_val); i++)
    {
        Serial.println(data[i]);
        buffer = data[i];
        buffer = buffer << ((i - begin_val) * 8);
        value += buffer;
    }
    return value;
}