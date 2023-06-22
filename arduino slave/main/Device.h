#pragma once 

#include "Arduino.h"
#include "config.h"
struct SlaveResponse {
    uint8_t buffer[20]={};
    uint8_t size=0;

};

class Device {
  public:
    int time = 0;
    uint8_t id=0;
    uint8_t subscription=0;
    int current_value =0;
    int previous_value =0;
    uint8_t status=0;
    uint8_t acknowledge=0; // acknowledge byte value
    uint8_t current_behaviour=1;


    uint8_t command;
    volatile uint8_t receivedBytes[RECEIVED_COMMAND_MAX_BYTES];
    volatile uint8_t receivedByteIdx = 0;
    volatile uint8_t pendingCommand[RECEIVED_COMMAND_MAX_BYTES];
    volatile uint8_t pendingCommandLength = 0;
    uint8_t my_addr;

    uint8_t mode = 0;
    bool deco_btn = 1;
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
    virtual SlaveResponse getResponse();
    /*
     * expectedReceiveLength(REGISTERID)
     * Returns the number of bytes to receive for a given
     * command or register, REGISTERID
     */
    uint8_t expectedReceiveLength(uint8_t commandId);
    /*
     *Method to get the request from wire.h library
    */
    void i2cRequest();
    /*
     *Method to get the receive from wire.h library
    */
    void i2cReceive(int bytes);

    /*
     * process(BUFFER, BUFLEN)
     * Process incoming data from master. 
     */
    virtual void process( );
    void changeAddr(uint8_t addr);

    void deconnect();
    void connect();
    void tick();
//behaviour handling 

    virtual void behaviour1 ();
    virtual void behaviour2 ();
    virtual void behaviour3 ();
    void (Device::*behaviour)() =nullptr;

    void behav();

    virtual void update_behav(uint8_t i=0);

    virtual void update_param();

    void update_global_subjects();

    uint8_t grap_subject( );
    virtual void update_subject();

    bool is_subscribe();

};

uint8_t * int_to_bytesarray(int value);
int bytesArraytoInt(volatile uint8_t* data,uint8_t len,uint8_t begin_val);