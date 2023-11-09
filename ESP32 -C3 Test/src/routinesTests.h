#include <utilities.h>
uint8_t buff[50];

uint8_t data[5];

device *my_nodes[MAX_DEVICE];
int mydevice_number = 0;


/*#############################################*/
/*######              TEST 1            #######*/
/*#############################################*/

// Test the connection of one node to the controller, with the 3 commands. Indice TEST=1

int test_connexion() 
{
    int out = 0;
    Serial.println("Routine de test numéro 1, connexion d'une device");
    if (digitalRead(SAP))
    {
        out = send_command(0x08, "ping");
        delay(5);
        if (!out)
        {
            Serial.println("Ping has been received at 0x08 apearing the new device if ack");
            receive_data(0x08, buff, 1);
            if (buff[0] == 1)
            {
                Serial.println("ACK is 1");
                Serial.println("Creating a new device");

                my_nodes[mydevice_number] = new device();
                mydevice_number++;
                Serial.println("Sending change addr command");

                out = send_command(0x08, "change_addr", 0x09);
                delay(5);

                if (!out)
                {
                    Serial.println("Change addr was received checking if ACK is 1");
                    my_nodes[0]->addr = 0x09;
                    receive_data(0x09, buff, 1);
                    if (buff[0] == 1)
                    {
                        Serial.println("ACK is 1 after change addr");

                        Serial.println("Sending get info command");

                        out = send_command(0x09, "get_info");
                        delay(5);

                        if (!out)
                        {
                            Serial.println("Change addr was received checking if ACK is 1");
                            receive_data(0x09, buff, 5);
                            if (buff[0] == 1)
                            {
                                Serial.print("Subscription : ");
                                Serial.println(buff[3]);

                                Serial.print("Behaviour : ");
                                Serial.println(buff[4]);

                                int id = bytesArraytoInt(buff, 2, 1);
                                Serial.print("Id: ");
                                Serial.println(buff[4]);

                                return 1;
                            }
                            else
                            {
                                                        Serial.println("ACK is 0 after get_info");
                        return 0;
                            }
                        }
                        else
                        {
                                                    Serial.println("get_info was not received");
                        return 0;
                        }
                    }
                    else
                    {
                        Serial.println("ACK is 0 after change addr");
                        return 0;
                    }
                }
                else
                {
                    Serial.println("change_addr was not received");
                    return 0;
                }
            }
            else
            {
                Serial.println("ACK is 0");
                return 0;
            }
        }
        else
        {
            Serial.println("ping was not received");
            return 0;
        }
    }
}

/*#############################################*/
/*######              TEST 2            #######*/
/*#############################################*/


// Test the connection of one node to the controller and check if a deconnection is asked through SAP to decoonect the device. Indice TEST=2

int test_connexion_deconnexion() 
{

    Serial.println("Routine de test numéro 2");

    return 1;
}