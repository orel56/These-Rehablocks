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


uint8_t size_list = 6;

uint16_t hex_val;

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
  int x = 0;
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
          // x=bytesArrayToInt(this->reponse_buffer.buffer,INFO_SIZE);
          x = 0;
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
  int x = 0;
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

      error = I2Cmaster::send_command(bus_addrs[k], "get_info", {}, 0);
      
      if (!error)
      {
        I2Cmaster::receive_data(bus_addrs[k], 3);
        // x=bytesArrayToInt(this->reponse_buffer.buffer,2);
        x = 0;
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
{
  Serial.println("in set function");
  my_handler->filter_list("actuator");
  uint8_t error = 0;
  uint8_t *data;
  if (!(my_handler->size))
  {
    for (int i = 0; i < my_handler->size_buffer; i++)
    {
        
        if ((my_handler->filter_buffer[i])->to_be)
        {
          data = int_to_bytesarray((my_handler->filter_buffer[i])->current_value, 4);
          error = I2Cmaster::send_command((my_handler->filter_buffer[i])->addr, "set_value", data, 4, 0x03);
          if (!error)
          {
            this->receive_data((my_handler->filter_buffer[i])->addr, 1);
            if (reponse_buffer.buffer[0] == 0x00)
            {
              my_handler->update_value((my_handler->filter_buffer[i])->addr, 0, true);
            }
          }
        }
    }
  }
}