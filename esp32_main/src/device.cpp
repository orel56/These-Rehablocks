#include <device.h>

Device::Device(){
    this->addr=0x08;
    this->current_value=0;
    this->quarantine=0;
    this->type=2;
    this->to_be=0;
};

Device::Device(uint8_t addr, uint8_t id,uint8_t subscription, uint8_t behaviour){
    this->addr=addr;
    this->current_value=0;
    this->quarantine=0;
    this->id=id;
    this->subscription=subscription;
    this->type=id>>7;
    this->to_be=0;
    this->current_behaviour=behaviour;
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