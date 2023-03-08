#include "Arduino.h"

struct SlaveResponse {
    uint8_t buffer[20]={};
    uint8_t size=0;

};

struct addrTab {
  uint8_t addrs[126];
  uint8_t size;
};

class I2Cmaster {
  public:
    struct SlaveResponse reponse_buffer;
    I2Cmaster();

    /*
     * getResponse -- returns an appropriate buffer, 
     * and its length, according to whatever commands 
     * have been received prior, or defaults.
     *
     * SlaveResponse is just a struct with:
     *   - uint8_t * buffer;
     *   - uint8_t size;
     */
    uint8_t send_command(uint8_t addr,char* command,uint8_t * data);

    // updtae reponse buffer with the new data received by salve at addr
    void receive_data(uint8_t addr, uint8_t bytesToBeReceived);
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

    uint8_t apering_process();

    void sendReady();

};
