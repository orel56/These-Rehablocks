/* Device
 * A class to centralize our peripheral's state
 * and message processing.
 */
#include "Device.h"



Device::Device()
{
  this->my_addr = EEPROM.read(0x00);

#ifdef INFO
  this->subscription = INFO_SUBSCRIPTION;
  this->production = INFO_PRODUCTION;
#endif
#ifndef INFO_BEHAVIOUR
  this->update_behav();
#else
  this->update_behav(INFO_BEHAVIOUR);
#endif
  this->init_received_subject();
};

SlaveResponse Device::getResponse()
{
  SlaveResponse response;
  response.buffer[0] = this->acknowledge;
  if (command == 2)
  {
    uint8_t bytes[4];
    uint8_t bytes2[8];

    intToBytesArray(this->id,bytes);
    response.buffer[1] = bytes[0];
    response.buffer[2] = bytes[1];
    longlongToBytesArray(this->subscription,bytes2);
    response.buffer[3] = bytes2[0];
    response.buffer[4] = bytes2[1];
    response.buffer[5] = bytes2[2];
    response.buffer[6] = bytes2[3];
    response.buffer[7] = bytes2[4];
    response.buffer[8] = bytes2[5];
    response.buffer[9] = bytes2[6];
    response.buffer[10] = bytes2[7];
    longlongToBytesArray(this->production,bytes2);
    response.buffer[11] = bytes2[0];
    response.buffer[12] = bytes2[1];
    response.buffer[13] = bytes2[2];
    response.buffer[14] = bytes2[3];
    response.buffer[15] = bytes2[4];
    response.buffer[16] = bytes2[5];
    response.buffer[17] = bytes2[6];
    response.buffer[18] = bytes2[7];

    response.buffer[19] = this->current_behaviour;
    response.size = 20;
  }
  else if (command == 3)
  {
    response=this->status;
  }
  else
  {
    response.size = 1;
    return response;
  }
  return response;
};

uint8_t Device::expectedReceiveLength(uint8_t commandId)
{
  if (commandId == 0x01 || commandId == 0x05)
  {
    return 2;
  }
  else if (commandId == 0x04)
  {
    return 7;
  }
  else if (commandId == 0x06)
  {
    return 6;
  }
  else
  {
    return 1;
  }
};

void Device::process()
{
  this->command = this->pendingCommand[0];
  if (this->mode == 0)
  {
    if (command == 0x02)
    {
      this->acknowledge = 1;
      // Led is switched to off to inform user the device is connected
      digitalWrite(USR_LED, LOW);
      this->mode = 2;

    }
    else if (command == 0x01)
    {
      this->acknowledge = 1;
      this->changeAddr(this->pendingCommand[1]);
    }
    else if (command == 0x00)
    {
      this->acknowledge = 1;
      //ping was received during connexion process thus SAP is not needed 
      digitalWrite(SAP, LOW);
      pinMode(SAP,INPUT);
    }
  }
  else if (this->mode == 2)
  {
    if (command == 0x03)
    {

      this->acknowledge = 1;
      this->get_status();
    }
    else if (command == 0x04) // publish subjects
    {

      this->acknowledge = 1;
      this->acknowledge = this->grap_subject();
    }
    else if (command == 0x05) // update behaviour
    {
      this->acknowledge = 1;
      this->update_behav();
    }
    else if (command == 0x06)
    {
      this->acknowledge = 1;
      this->update_param();
    }
    else
    {
      this->acknowledge = 1;
    }
  }
  else if (this->mode == 1)
  {
    Serial.println(command);
    if (command == 0x00)
    {
      Serial.println("erasing addr");
      EEPROM.write(0x00, 0x08);
      digitalWrite(USR_LED, LOW);
      digitalWrite(SAP, LOW);
      pinMode(SAP,INPUT);
      this->mode = 3;
    }
  }
};

uint8_t Device::grap_subject()
{
  if (this->pendingCommand[1])
  {
    // general interest
    this->update_global_subjects();
    return 1;
  }
  else
  {
      return (this->update_subject());
  }
}



void Device::update_global_subjects()
{
  uint8_t subject = this->pendingCommand[2];
  int value = 0;
  switch (subject)
  {
  case 1:
    value = bytesArraytoInt(this->pendingCommand, this->pendingCommandLength, 3);
    this->time = value;
    break;
  default:
    break;
  }
}

uint8_t Device::update_subject()
{
  Serial.print("I need to receive : "); 
  Serial.print(this->received_subject_nbr);
  Serial.println("subjects");
  for (int i = 0; i < this->received_subject_nbr; i++)
  {
    Serial.print("subject id received is : ");
    Serial.println(this->pendingCommand[2]);
    if (this->receivedSubjects[i]->id == this->pendingCommand[2])
    {    Serial.print("ok");

      (this->receivedSubjects[i])->old_value = (this->receivedSubjects[i])->value;
      (this->receivedSubjects[i])->value = bytesArraytoInt(pendingCommand, 4, 3);

      Serial.print("new value is :");
      Serial.println(this->receivedSubjects[i]->value);
      return 1;
    }
  }
  return 0;
}

void Device::init_received_subject()
{
  int n = sizeof(this->subscription);

  int id_sub = 0;
  for (int i = 0; i < n; i++)
  {
    id_sub = (this->subscription & (0xff << ( i * 8))) >> 8*i;;
    if (id_sub)
    {
      this->received_subject_nbr++;
      this->receivedSubjects[received_subject_nbr - 1] = new Subject(id_sub);
    }
  }
}

void Device::changeAddr(uint8_t addr)
{
  Serial.println("change addr");
  this->my_addr = addr;
  TWAR = addr << 1 |1;
  EEPROM.write(0x00, this->my_addr);
}

void Device::tick()
{
  if (pendingCommandLength)
  {

    // oh my, we've received a command!

    // if you're going to be very slow in processing this,
    // you could copy the contents of pendingCommand[]
    // over to yet another buffer.
    // Here we just do it "real time" for simplicity

    // 1) process that command
    this->process();

    // 2) zero that flag, so we don't process multiple times
    pendingCommandLength = 0;
  }

  // do anything else that needs doin'
  this->behav();
  // main thing is that you must process any pendingCommand
  // before the next full command bytes come in through the wire
}

void Device::deconnect()
{
  pinMode(SAP,OUTPUT);
  digitalWrite(USR_LED, HIGH);
  digitalWrite(SAP, HIGH);
  this->mode = 1;
}
  




void Device::i2cRequest()
{
  // get the response (the this knows what to say)
  SlaveResponse resp = this->getResponse();
  // write it to the out buffer

  Wire.write(resp.buffer, resp.size);
}

void Device::i2cReceive(int bytes)
{
  uint8_t msgLen = 0;
  // loop over each incoming byte
  this->acknowledge = 0;
  for (int i = 0; i < bytes; i++)
  {
    // stick that byte in our receive buffer
    this->receivedBytes[this->receivedByteIdx] = Wire.read();
    // now, we're sure we have _at least_ one byte in the buffer
    if (!msgLen)
    {
      // this was the first byte of a message,
      // so we couldn't know the
      // expected message length until now...
      // ask our device what to expect:
      msgLen = this->expectedReceiveLength(this->receivedBytes[0]);
    }
    this->receivedByteIdx++; /* increment in-byte counter */

    if (this->receivedByteIdx >= msgLen)
    {

      // we have a complete request/command in our buffer!

      // 1) copy that into our pending commands buffer
      // could use memcpy, we do it manual style:
      for (uint8_t i = 0; i < msgLen; i++)
      {
        this->pendingCommand[i] = this->receivedBytes[i];
      }
      // 2) tell the main loop we've got something
      // of interest in pending cmd buffer
      this->pendingCommandLength = msgLen;
      // 3) zero our in-bytes buffer, to start
      // the next message
      this->receivedByteIdx = 0;

      // 4) zero our expected msgLen, so we'll refresh it
      // for the next command
      msgLen = 0;
    }
  }
    this->tick();

}

void Device::behaviour1(){};
void Device::behaviour2(){};
void Device::behaviour3(){};
void Device::behav()
{
  (this->*behaviour)();
  this->produce_subjects();
}

void Device::update_behav(uint8_t i)
{
  if (!i)
  {
    i = pendingCommand[1];
    this->current_behaviour = i;
  }
  else
  {
    this->current_behaviour = i;
  }

  switch (i)
  {
  case 1:
    behaviour = &Device::behaviour1;
    break;
  case 2:
    behaviour = &Device::behaviour2;
    break;
  case 3:
    behaviour = &Device::behaviour3;
    break;
  default:
    this->current_behaviour = 1;
    behaviour = &Device::behaviour1;
    break;
  }
};

void Device::update_param()
{ // use pendingcommand to access the data aquired from master
}
void Device::get_status()
{
  SlaveResponse resp;
  resp.buffer[0] = this->acknowledge;
  resp.buffer[1] = this->producedSubjects[0]->value;
  resp.size = 2;
  uint8_t bytes[4];
  for (int i = 1; i < this->produced_subject_nbr; i++)
  {
    resp.buffer[resp.size] = uint8_t(this->producedSubjects[i]->id);
    resp.size+=1;
    intToBytesArray(this->producedSubjects[i]->value,bytes);

    for (int k=0;k<4;k++){
      resp.buffer[resp.size] = bytes[k];
      resp.size+=1;
    }

  }
  this->status = resp;
};

void Device::setup(){};

void Device::init_produced_subject()
{
}

void Device::produce_subjects() {}