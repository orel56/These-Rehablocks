#include <stdint.h>
#include <mat.h>

struct subject {
    uint8_t id;
    int value;
    int prev_value;
    uint8_t is_produced_by[10];
    uint8_t are_subscribed[10];
    bool to_be_sent;
};

typedef struct subject Subject;


typedef struct list_sub {
    Subject* subject;
    struct list_sub* next_link=NULL;
    struct list_sub* prev_link=NULL;
} list_subject;

void push(list_subject **list_sub,Subject* subject);

void retreive(list_subject **list_sub,uint8_t addr);

Subject* access(list_subject **list_sub,uint8_t val);

void clear(list_subject ** list_sub);
