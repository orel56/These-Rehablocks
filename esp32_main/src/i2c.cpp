#include <i2c.h>

volatile uint8_t receivedBytes[RECEIVED_COMMAND_MAX_BYTES];

uint8_t data2[50];

int send_command(uint8_t addr, const char *command, uint8_t value, uint8_t* value2)
{
    uint8_t bytes;
    int out=1;
    if (command == "ping")
    {
        data2[0] = 0x00;
        bytes = 1;
        Serial.println("setup ping");
    }
    else if (command == "change_addr")
    {
        data2[0] = 0x01;
        data2[1] = value;
        bytes = 2;
    }
    else if (command == "get_info")
    {
        data2[0] = 0x02;
        bytes = 1;
    }
    else if ( command == "get_status"){
        data2[0] = 0x03;
        bytes=1;
    }
    else if (command=="publish_subject")
    {
        Serial.println("publishing subject wirtin data");
        data2[0]=0x04;
        data2[1]=0x00;
        data2[2]=value; 
        data2[3]=value2[0];
        data2[4]=value2[1];
        data2[5]=value2[2];
        data2[6]=value2[3];
        bytes=7;
    }
    else
    {
        Serial.println("Don't understand the command");
    }

 Wire.beginTransmission(addr);
    for (int i = 0; i < bytes; i++)
    {
        Wire.write(data2[i]);
    }
    out = Wire.endTransmission();
    return out;
}

void receive_data(uint8_t addr, uint8_t *data_buffer, uint8_t bytesToBeReceived)
{
    uint8_t dataRead;
    uint8_t bytesReceived = Wire.requestFrom(addr, bytesToBeReceived);
    for (int i = 0; i < bytesReceived; i++)
    {
        if(Wire.available()){
            dataRead= Wire.read();
            Serial.println(dataRead);
            data_buffer[i] = dataRead;
        }
    else{
        Serial.print("not available;");
    }
    }
}
