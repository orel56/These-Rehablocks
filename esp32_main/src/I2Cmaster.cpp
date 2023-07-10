/* MyI2CPeripheral
 * A class to centralize our peripheral's state
 * and message processing.
 */

#include "I2Cmaster.h"


unsigned char *int_to_bytesarray(int value, uint8_t size)
{

  static unsigned char bytes[4];
  for (int i = 0; i < 4; i++)
  {
    bytes[i] = (value & (0x000000ff << (2 * i * 4))) >> 8 * i;
  }
  return bytes;
}

int bytesArrayToInt(unsigned char *bytes, unsigned char len)
{
  int value = 0;
  int buffer = 0;
  for (int i = (len - 4); i < len; i++)
  {
    buffer = bytes[i];
    Serial.println(buffer);
    buffer = buffer << ((i-1) * 8);
    Serial.println(buffer);
    value += buffer;
  }

  return value;
}



I2Cmaster::I2Cmaster(){};

uint8_t I2Cmaster::send_command(uint8_t addr, uint8_t commandCode, uint8_t *data, uint8_t bytes)
{
  
  Wire.beginTransmission(addr);
  Wire.write(commandCode);
  for (int i = 0; i < bytes; i++)
  {
    Wire.write(data[i]);
  }
  int out = Wire.endTransmission();

  return out;
}

void I2Cmaster::receive_data(uint8_t addr, uint8_t bytesToBeReceived = 20)
{
  const uint8_t bytesReceived = Wire.requestFrom(addr, bytesToBeReceived);
  I2Cmaster::reponse_buffer.size = bytesReceived;
  for (int i = 0; i < bytesReceived; i++)
  {
    uint8_t dataRead = Wire.read();
    I2Cmaster::reponse_buffer.buffer[i] = dataRead;
  }
}

std::vector<byte> I2Cmaster::scan()
{
  byte error, address;
  std::vector<byte> findAddr;
  for (address = 9; address < 127; address++)
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      if (address < 16)
        findAddr.push_back(address);
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.println(address, HEX);
    }
  }
  return findAddr;
};

uint8_t I2Cmaster::ping(uint8_t addr)
{
  byte error;
  Wire.beginTransmission(addr);
  Wire.write(0x00);
  error = Wire.endTransmission();
  return error;
}

void I2Cmaster::apering_process(DeviceHandler *my_handler)
{
  Serial.println("apearing the new device");
  byte error = 0;
    // 0x08 device received ping
    uint8_t new_addr = my_handler->ask_free_addr();
    Serial.println("ping has been received, changing addr 0x08 for :  ");
    Serial.println(new_addr);
    uint8_t data[1];
    data[0] = new_addr;
    error = I2Cmaster::send_command(0x08, 0x01, data, 1);

    if (!error)
    {
      Serial.println("change command has been received  ");
      
      I2Cmaster::receive_data(new_addr, 1);

      if (I2Cmaster::reponse_buffer.buffer[0] == 0x01)
      {
        Serial.println("getting informations from new device");

        error = I2Cmaster::send_command(new_addr, 0x02, {}, 0);

        if (!error)
        {
          

          I2Cmaster::receive_data(new_addr, 5);

          my_handler->add_new_device(new_addr, this->reponse_buffer.buffer);
        }
      }
    }
  }

void I2Cmaster::disconnexion_process(DeviceHandler* my_handler){
  uint8_t error = 0;
  uint8_t addr =0;
   if ((my_handler->size))
  {
    for (int i = 0; i < my_handler->size; i++)
    {
      //Serial.println("Sending get_value command : ");
      addr=(my_handler->access_dev(i))->addr;
      error=this->ping(addr);
      delay(5);
      if (!error)
      { 
        if(!digitalRead(SAP)){
          //DEconnected device find 
          my_handler->delete_device(addr);
          break;
        }
      }
        else {
          Serial.println("error ping");
        }
        }
      }
      }
void I2Cmaster::i2c_init(DeviceHandler *my_handler)
{
  Serial.println("in init");
  Wire.begin();
  Wire.setTimeOut(100);
  std::vector<uint8_t> bus_addrs = this->scan();
  Serial.println("Number of device found : ");
  Serial.println(bus_addrs.size());
  for (int k = 0; k < bus_addrs.size(); k++)
  {
    Serial.println("device found at addrs :");
    Serial.println(bus_addrs[k]);

    byte error = 0;
    error = I2Cmaster::ping(bus_addrs[k]);

    if (!error)
    {

      error = I2Cmaster::send_command(bus_addrs[k],0x02, {}, 0);
      
      if (!error)
      {
        I2Cmaster::receive_data(bus_addrs[k], 3);

        Serial.println("this device will be added in the device handler");
        my_handler->add_new_device(bus_addrs[k],this->reponse_buffer.buffer);
      }
    }
  }
}

void I2Cmaster::i2c_get_status(DeviceHandler *my_handler)
{
  Serial.println("in get function");
  uint8_t data[0];
  Device* tmp = NULL;
  uint8_t addr;
  uint8_t prev_addr;

  if(my_handler->size){

    tmp=(my_handler->access_dev(0));
    addr=tmp->addr;
    this->send_command(addr,0x03,data,0);
    delay(10);
    for (uint8_t i = 1; i < my_handler->size; i++)
    {
      // on envoie la commande à la ième device
      prev_addr=addr;
      tmp=(my_handler->access_dev(i));
      addr=tmp->addr;
      this->send_command(addr,0x03,data,0);

      //on reccupère les data de la i-1 ème device en attendant le traitement de la commande par le bloc 

      this->receive_data(prev_addr,MAX_BYTES);
      my_handler->update_value(prev_addr,this->reponse_buffer);
    }
    delay(10);
    this->receive_data(addr,MAX_BYTES);
    my_handler->update_value(addr,this->reponse_buffer);
  }
  
};

void I2Cmaster::i2c_set(DeviceHandler *my_handler)
{   uint8_t error = 0;
    uint8_t *data;
    Subject *sub;
    Device *dev;
    std::list<uint8_t>::iterator Iter;
    uint8_t available_subjects = (my_handler->list_subject).size();


    for (uint8_t k = 0; k < available_subjects; k++)
    {
        sub = my_handler->access_subject(k, -1);
        if (sub->to_be_sent){
          for (uint8_t i=0;i<my_handler->size;i++){
            dev=my_handler->access_dev(0,i);
            if(dev->is_subscribe(sub->id)){
              data = this->generate_data_subject(sub);
              error = this->send_command(dev->addr,0x04, data, 6);
              if (!error)
              {
                delay(10);
                this->receive_data(dev->addr, 1);
                if ((this->reponse_buffer).buffer[0]){
                  Serial.println("acknowledge de la part de la device");
                }
                else {
                  Serial.println("la device n'a pas acknowledge le sujet");
                }
              }
              else{
                  Serial.println("Subject data wasn't received by device : ");
                  Serial.println(dev->addr);
              }

          }


        }
    }
}
    uint8_t datas[6]={1,1,1,2,3,0};
    error=this->send_command(0x00,0x04,data,6);
    if (!error){
      Serial.println("general subject id 1 was sent without error");
    }
    else {
      Serial.println("An error occured when sending general subject id 1");

    }
} 

uint8_t* I2Cmaster::generate_data_subject(Subject* sub){
  static uint8_t data[6];
  uint8_t *buff;
  data[0]=0;
  data[1]=sub->id;
  buff=int_to_bytesarray(sub->value,4);
  data[2]=buff[0];
  data[3]=buff[1];
  data[4]=buff[2];
  data[5]=buff[3];
  return data;
}