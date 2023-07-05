#include <device.h>

struct subject {
    uint8_t id;
    int value;
    int prev_value;
    uint8_t is_produced_by[10];
    uint8_t are_subscribed[10];
    bool to_be_sent;
};

typedef struct subject Subject;



Device::Device(){
    this->addr=0x08;
    this->current_value=0;
    this->type=2;
};

Device::Device(uint8_t addr, int id,uint8_t subscription, uint8_t behaviour){
    this->addr=addr;
    this->current_value=0;
    this->id=id;
    this->extract_id_info();
    this->subscription=subscription;
    this->current_behaviour=behaviour;
}


void Device::extract_id_info(){
    this->type=(this->id)>>15;
    this->sub_type=(this->id)>>10 & 0b011111;
    this->familly=(this->id)>>7 & 0b000000111;
    this->sub_fam=(this->id)>>4 & 0b000000000111;
}


void push(list_device **list_dev,Device* device){
    list_device *element = (list_device *) malloc(sizeof(list_device));
    if(!element) exit(EXIT_FAILURE); /* Si l'allocation a échoué. */
   if(*list_dev){
        (*list_dev)->next_link=element;
        element->device = device;
        element->prev_link = *list_dev;
        }
    else{
    element->next_link=NULL;
    element->device = device;
    element->prev_link = NULL;
    }
    *list_dev = element;
}

void retreive(list_device **list_dev,uint8_t addr){
    list_device* tmp=*list_dev;
    while(tmp){
        if (((tmp)->device)->addr==addr){
            if(tmp->prev_link){
                if(tmp->next_link){

                (tmp->prev_link)->next_link=tmp->next_link;
                (tmp->next_link)->prev_link=tmp->prev_link;

                free(tmp);
                }
                else{
                // queue de liste
                (tmp->prev_link)->next_link=NULL;
                *list_dev=tmp->prev_link;
                free(tmp);
                }
            }
            else{
                // tete de liste 
                (tmp->next_link)->prev_link=NULL;
                free(tmp);
            }
            break;
        }
        else {
            tmp=tmp->prev_link;
        }
    }
}

Device* access(list_device **list_dev,uint8_t val,const char * attr){
    list_device* tmp=*list_dev;
    if (attr=="addr"){
        while(tmp){
            if (((tmp)->device)->addr==val){
                return ((tmp)->device);
                break;
            }
            else {
                tmp=tmp->prev_link;
            }
        }
    }
    else if (attr=="id"){
          while(tmp){
            if (((tmp)->device)->id==val){
                return ((tmp)->device);
                break;
            }
            else {
                tmp=tmp->prev_link;
            }
        }
    }

    return NULL;
}

void clear(list_device ** list_dev){


}