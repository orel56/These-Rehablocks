#include "device_handler.h"

std::list<knowledge> database_type;
std::list<knowledge> database_fam;


int bytesArraytoInt(uint8_t *data, uint8_t len, uint8_t begin_val)
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

DeviceHandler::DeviceHandler()
{
    this->size_buffer = 0;
    this->size = 0;
    init_database_fam(database_fam);
    init_database_type(database_type);
}

/* DeviceHandler::DeviceHandler(list_device* list_dev, uint8_t size){
  this->list_dev=NULL;
  for (int i=0;i<size;i++){
    push(&(this->list_dev),);
  }
  this->size_buffer=0;
  this->size=size;
} */

DeviceHandler::~DeviceHandler()
{
}

void DeviceHandler::add_new_device(uint8_t addr,uint8_t * buffer)
{
    if (this->size < MAX_DEVICE)
    {
        Serial.println("there is enought place for the device");
        Serial.println("Device is an actuator");
        uint8_t subscription=buffer[3];
        uint8_t behaviour = buffer[4];

        int id = buffer[1];;
        int tmp = buffer[2];;
        tmp = tmp << ((2-1) * 8);
        id += tmp;

        (this->list_device).push_back(new Device(addr, id,subscription, behaviour));
        this->update_subjects();
    }
    this->size += 1;
};

void DeviceHandler::update_subjects(){
    uint8_t subscriptions = ((this->list_device).back())->subscription;
    uint8_t id_sub=0;
    if (subscriptions){
        for (uint8_t i=0;i<8;i++){
            id_sub=subscriptions & (0b10000000 >> i);
            if(id_sub){
                Subject* sub=this->access_subject(-1,id_sub);
                if (sub !=NULL){
                    sub->are_subscribe.push_back(((this->list_device).back())->addr);
                }
                else{
                    this->add_new_subject(id_sub);
                    sub=(this->list_subject).back();
                    sub->are_subscribe.push_back(((this->list_device).back())->addr);
                }
            }            
        }
    }
    std::list<uint8_t> product;
    std::list<knowledge>::iterator Iter; 
    for (Iter = (database_type).begin(); Iter != database_type.end(); Iter++){
        if ((*Iter).value==((this->list_device).back())->type){
            product=(*Iter).ids;
            break;

        }
    }
    std::list<Subject*>::iterator Iter2; 
    std::list<uint8_t>::iterator Iter3; 
    for (Iter2 = (this->list_subject).begin(); Iter2 != (this->list_subject).end(); Iter2++){
        id_sub=(*Iter2)->id;
        for (Iter3 = product.begin(); Iter3 != product.end(); Iter3++){
            if(id_sub==(*Iter3)){
                ((*Iter2)->is_produced_by).push_back(((this->list_device).back())->addr);
                break;
            }
    }    
}
}

void DeviceHandler::add_new_subject(uint8_t id){
    (this->list_subject).push_back(new Subject);
    ((this->list_subject).back())->id=id;
    ((this->list_subject).back())->value=0;
    ((this->list_subject).back())->prev_value=0;
    ((this->list_subject).back())->to_be_sent=0;
    std::list<knowledge>::iterator Iter; 
    for (Iter = (database_fam).begin(); Iter != database_fam.end(); Iter++){
        if ((*Iter).value== id){
            ((this->list_subject).back())->substitute=(*Iter).ids;
            break;

        }
            }
}

Subject* DeviceHandler::access_subject(int8_t position,int8_t id){
    std::list<Subject*>::iterator Iter; 
    if (id!=-1){
    for (Iter = (this->list_subject).begin(); Iter != this->list_subject.end(); Iter++){
        if ((*Iter)->id== id){
            return *Iter;
        }
    }
    }
    else {
    int i=0;
    for (Iter = (this->list_subject).begin(); Iter != this->list_subject.end(); Iter++){
        if (i==position){
            return *Iter;
        }
        i++;
    }

    }
    return NULL;
}

uint8_t DeviceHandler::ask_free_addr()
{
    if (this->size == 0)
    {
        return 0x09;
    }
    else if (this->next_addr!=0){
        uint8_t buf = this->next_addr;
        this->next_addr=0;
        return buf;
    }
    return 0x09 + this->size;
};

void DeviceHandler::filter_list(const char *filter)
{
    this->size_buffer = 0;
    Device *tmp = NULL;
    if (!(strcmp(filter, "sensor")))
    {
        for (uint8_t i = 0; i < this->size; i++)
        {
            tmp = this->access_dev(0x09 + i);
            if (tmp->type == 0)
            {
                this->filter_buffer[i] = tmp;
                this->size_buffer += 1;
            }
        }
    }
    else if (!(strcmp(filter, "actuator")))
    {
        for (uint8_t i = 0; i < this->size; i++)
        {
            tmp = this->access_dev(0x09 + i);
            if (tmp->type == 1)
            {
                this->filter_buffer[i] = tmp;
                this->size_buffer += 1;
            }
        }
    }
}

Device * DeviceHandler::access_dev(uint8_t addr, int8_t position){
    std::list<Device*>::iterator Iter; 
    if (position==-1){
    for (Iter = (this->list_device).begin(); Iter != this->list_device.end(); Iter++){
        if ((*Iter)->addr== addr){
            return *Iter;
        }
    }
    }
    else {
        int i=0;
        for (Iter = (this->list_device).begin(); Iter != this->list_device.end(); Iter++){
        if (i==position){
            return *Iter;
        }
        i++;
    }
    }
    return NULL;
}

void DeviceHandler::update_value(uint8_t addr, SlaveResponse resp)
{
    Serial.println("updtating value of addr : ");
    Serial.println(addr);
    Device *tmp = NULL;
    int buf=0;
    tmp = this->access_dev(addr);
    if (tmp)
    {
        if (resp.buffer[0]==2)
        {
            tmp->current_value = tmp->previous_value;
        }
        else
        {   // à modifier pour plus d'abstraction
            tmp->status = resp.buffer[1];
            if (tmp->sub_type==1){
                //accelero 
                tmp->prev_x=tmp->x;
                buf = bytesArraytoInt(resp.buffer,6,2);
                tmp->x=reinterpret_cast<float&>(buf);
                tmp->prev_y=tmp->y;
                buf = bytesArraytoInt(resp.buffer,10,6);
                tmp->y=reinterpret_cast<float&>(buf);
            }
            else {
                tmp->previous_value=tmp->current_value;     
                tmp->current_value=bytesArraytoInt(resp.buffer,6,2);
            }
        }
    }
}

/* void DeviceHandler::put_in_quarantine(uint8_t addr)
{
    Serial.println("putting in quarantine the device at addr : ");
    Serial.println(addr);
    Device *tmp = NULL;
    tmp = access(&list_dev, addr);
    if (tmp)
    {
        tmp->quarantine = 1;
        this->check_device_flag = 1;
    }
} */

void DeviceHandler::delete_device(uint8_t addr)
{
    std::list<Device*>::iterator Iter; 
    for (Iter = (this->list_device).begin(); Iter != this->list_device.end(); Iter++){
        if ((*Iter)->addr== addr){
            (this->list_device).erase(Iter);
            this->next_addr=addr;
            break;
        }
    }    
    this->size -= 1;
    this->check_device_flag = 0;
}
