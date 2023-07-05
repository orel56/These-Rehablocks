#include "subject.h"

void retreive(list_sub **list_sub,uint8_t addr){
    
}

Subject* access(list_sub **list_sub,uint8_t val){
    
}

void clear(list_sub ** list_sub){
    
}

void push(list_subject **list_sub,Subject* subject){
    list_subject *element = (list_subject *) malloc(sizeof(list_subject));
    if(!element) exit(EXIT_FAILURE); /* Si l'allocation a échoué. */
    if(*list_sub){
        (*list_sub)->next_link=element;
        element-> subject = subject;
        element->prev_link = *list_sub;
        }
    else{
    element->next_link=NULL;
    element->subject = subject;
    element->prev_link = NULL;
    }
    *list_sub = element;
}
