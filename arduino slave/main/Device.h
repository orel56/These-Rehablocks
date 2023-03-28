#pragma once 

#include "Arduino.h"
#include "config.h"
struct SlaveResponse {
    uint8_t buffer[20]={};
    uint8_t size=0;

};

class Device {
  public:
    String command;
    String mycommands[3]={("ping"),("change_addr"),("get_info")};
    volatile uint8_t receivedBytes[RECEIVED_COMMAND_MAX_BYTES];
    volatile uint8_t receivedByteIdx = 0;

    volatile uint8_t pendingCommand[RECEIVED_COMMAND_MAX_BYTES];
    volatile uint8_t pendingCommandLength = 0;
    uint8_t my_addr=0x08;

    String mode = "";

    bool deco_btn = 0;
    uint8_t connect_follow=0;
    Device();

    /*
     * getResponse -- returns an appropriate buffer, 
     * and its length, according to whatever commands 
     * have been received prior, or defaults.
     *
     * SlaveResponse is just a struct with:
     *   - uint8_t * buffer;
     *   - uint8_t size;
     */
    SlaveResponse getResponse();
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
    virtual void process(volatile uint8_t * buffer, uint8_t len);

    virtual void doThings();

    void changeAddr(uint8_t addr);

    void deconnect();
    void connect();
    void tick();

};

class behavior {


};