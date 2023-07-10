#include <list>

typedef struct knowledge
{
    unsigned char value = 0; // type de la device 
    std::list<unsigned char> ids;
}knowledge;

void init_database_type(std::list<knowledge> database_type);

void init_database_fam(std::list<knowledge> database_fam);