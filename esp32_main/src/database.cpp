#include "database.h"

void init_database_type(std::list<knowledge> database_type){
    knowledge knowledge;
    knowledge.value=0b000000; //potar
    (knowledge.ids).push_back(0b00000100); //produit le sujet potar
    database_type.push_back(knowledge);
    knowledge.value=0b000001; //accelero
    (knowledge.ids).clear();
    (knowledge.ids).push_back(0b10000000);
    (knowledge.ids).push_back(0b01000000);
    (knowledge.ids).push_back(0b00100000);
    (knowledge.ids).push_back(0b00010000);
    database_type.push_back(knowledge);
    knowledge.value=0b100000; //led
    (knowledge.ids).clear();
    (knowledge.ids).push_back(0b00001000);
    database_type.push_back(knowledge);
    knowledge.value=0b100001; //buzz
    (knowledge.ids).clear();
    (knowledge.ids).push_back(0b00000010);
    database_type.push_back(knowledge);
}

void init_database_fam(std::list<knowledge> database_fam){
    knowledge knowledge;
    knowledge.value=0b10000000; //accelero
    (knowledge.ids).push_back(0b00000100); //produit le sujet potar
    database_fam.push_back(knowledge);
    knowledge.value=0b01000000; //accelero
    (knowledge.ids).clear();
    (knowledge.ids).push_back(0b00100000);
    (knowledge.ids).push_back(0b00010000);
    database_fam.push_back(knowledge);
    knowledge.value=0b00100000; //accelero
    (knowledge.ids).clear();
    (knowledge.ids).push_back(0b01000000);
    (knowledge.ids).push_back(0b00010000);
    database_fam.push_back(knowledge);
    knowledge.value=0b00010000; //accelero
    (knowledge.ids).clear();
    (knowledge.ids).push_back(0b00100000);
    (knowledge.ids).push_back(0b01000000);
    database_fam.push_back(knowledge);
    knowledge.value=0b00001000; //accelero
    (knowledge.ids).clear();
    (knowledge.ids).push_back(0b00000010);
    database_fam.push_back(knowledge);
    knowledge.value=0b00000100; //accelero
    (knowledge.ids).clear();
    (knowledge.ids).push_back(0b10000000);
    database_fam.push_back(knowledge);
    knowledge.value=0b00000010; //accelero
    (knowledge.ids).clear();
    (knowledge.ids).push_back(0b00001000);
    database_fam.push_back(knowledge);
}