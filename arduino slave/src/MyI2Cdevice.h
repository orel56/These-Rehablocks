#include "Arduino.h"

struct SlaveResponse {
    uint8_t buffer[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint8_t size=0;

};

struct addrTab {
  uint8_t addrs[126];
  uint8_t size;
};

class MyI2CPeripheral {
  public:
    char *command;
    uint8_t my_addr=0x08;
    MyI2CPeripheral();

    /*
     * getResponse -- returns an appropriate buffer, 
     * and its length, according to whatever commands 
     * have been received prior, or defaults.
     *
     * SlaveResponse is just a struct with:
     *   - uint8_t * buffer;
     *   - uint8_t size;
     */
    SlaveResponse getResponse(int val);



    /*
     * expectedReceiveLength(REGISTERID)
     * Returns the number of bytes to receive for a given
     * command or register, REGISTERID
     */
    uint8_t expectedReceiveLength(uint8_t forRegister);

    /*
     * process(BUFFER, BUFLEN)
     * Process incoming data from master. 
     */
    void process(volatile uint8_t * buffer, uint8_t len);

    void doThings(int *val);

    addrTab scan();

    void changeAddr(addrTab usedAddr);

};
