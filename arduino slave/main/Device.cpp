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
    intToBytesArray(this->id,bytes);
    response.buffer[1] = bytes[0];
    response.buffer[2] = bytes[1];
    response.buffer[3] = this->subscription;
    response.buffer[4] = this->current_behaviour;
    response.size = 5;
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
  Serial.println("processing");
  if (this->mode == 0)
  {
    if (command == 0x02)
    {
      Serial.println("command is get_info");
      this->acknowledge = 1;

      this->connect_follow++;
    }
    else if (command == 0x01)
    {
      Serial.println("command is change addr");
      this->connect_follow++;
      this->acknowledge = 1;
      this->changeAddr(this->pendingCommand[1]);
    }
    else if (command == 0x00)
    {
      Serial.println("command is ping and my addr is 0x08 meaning I 'am connecting");
      this->acknowledge = 1;
      this->connect_follow++;
    }
  }
  else if (this->mode == 2)
  {
    if (command == 0x03)
    {
      Serial.println("command is get_status");

      this->acknowledge = 1;
      this->get_status();
    }
    else if (command == 0x04) // publish subjects
    {
      Serial.println("command is publish_subject");

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
    if (is_subscribe())
    {
      Serial.print("I'am subscribe to the subject");
      Serial.println(this->pendingCommand[2]);
      this->update_subject();
      return 1;
    }
    else
    {
      return 0;
    }
  }
}

bool Device::is_subscribe()
{
  uint8_t tmp = this->pendingCommand[2];

  if ((tmp & (tmp - 1)) == 0)
  {

    tmp = (this->subscription >> __builtin_ctz(tmp)) & 1;
  }

  return tmp;
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

void Device::update_subject()
{
  for (int i = 0; i < this->received_subject_nbr; i++)
  {
    if (this->receivedSubjects[i]->id == this->pendingCommand[2])
    {
      (this->receivedSubjects[i])->old_value = (this->receivedSubjects[i])->value;
      (this->receivedSubjects[i])->value = bytesArraytoInt(pendingCommand, 4, 3);

    }
  }
}

void Device::init_received_subject()
{
  int n = sizeof(this->subscription) * 8;

  int id_sub = 0;
  for (int i = 0; i < n; i++)
  {
    id_sub = (this->subscription & (0b10000000 >> i));
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

void Device::connect()
{
  if (this->connect_follow < 3)
  {
    this->tick();
  }
  else
  {
    Serial.println("changing usr led to low, after connect follow reached 3");
    digitalWrite(USR_LED, LOW);
    digitalWrite(SAP, LOW);
    pinMode(SAP,INPUT);
    this->tick();

    this->mode = 2;
  }
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
  Serial.println("command is received");
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
  Serial.println("On termine le process de i2cReceive");
  if (this->mode == 0)
  {
    this->connect();
  }
  else if (this->mode == 2)
  {
    this->tick();
  }
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