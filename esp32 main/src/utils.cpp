
#include "string.h"

int search_str (char** list_str,char* str,int size_list){
    for (int i=0;i<size_list;i++){
        if(strcmp(list_str[i],str)==0){
            return i;
        }
    }
    return -1;
}
