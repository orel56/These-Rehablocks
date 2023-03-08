
#include "string.h"



struct SlaveResponse {
    uint8_t buffer[20]={};
    uint8_t size=0;

};

struct addrTab {
  uint8_t addrs[126];
  uint8_t size;
};


int search_str (char** list_str,char* str,int size_list){
    for (int i=0;i<size_list;i++){
        if(strcmp(list_str[i],str)==0){
            return i;
        }
    }
    return -1;
}
