#include "Arduino.h"
#include "utils.cpp"

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
    uint8_t send_command(uint8_t addr,const char* command,uint8_t * data, uint8_t bytes);

    // updtae reponse buffer with the new data received by salve at addr
    void receive_data(uint8_t addr, uint8_t bytesToBeReceived);
    /*
     * expectedReceiveLength(REGISTERID)
     * Returns the number of bytes to receive for a given
     * command or register, REGISTERID
     */
   
    addrTab scan();

    uint8_t apering_process();

    uint8_t ping(uint8_t addr);
};
