#define MIN_ADDR 0x09
#define MAX_ADDR 0x77

#define RECEIVED_COMMAND_MAX_BYTES 20

struct Tab{
uint8_t addrs[127];
int size=0;
};

struct slave{
int type;
uint8_t addr;

};

struct slaveList{
slave list[127];
int size;

};