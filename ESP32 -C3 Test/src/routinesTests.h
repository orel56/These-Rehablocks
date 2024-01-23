#include <utilities.h>
uint8_t buff[50];

uint8_t data[5];

uint8_t new_addr = 0x09;

device *my_nodes[MAX_DEVICE]; // device storage
int mydevice_number = 0;

Subject *my_subjects[50];
int my_sub_number = 0;

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
        Serial.println("SAP is 1");
        out = send_command(0x08, "ping");
        delay(5);
        Serial.print(" after delay out is : ");
        Serial.println(out);
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

                out = send_command(0x08, "change_addr", new_addr);
                delay(5);

                if (!out)
                {
                    Serial.println("Change addr was received checking if ACK is 1");
                    my_nodes[0]->addr = new_addr;
                    receive_data(new_addr, buff, 1);
                    if (buff[0] == 1)
                    {
                        Serial.println("ACK is 1 after change addr");

                        Serial.println("Sending get info command");

                        out = send_command(new_addr, "get_info");
                        delay(5);

                        if (!out)
                        {
                            Serial.println("Change addr was received checking if ACK is 1");
                            receive_data(new_addr, buff, 5);
                            if (buff[0] == 1)
                            {
                                Serial.print("Subscription : ");
                                Serial.println(buff[3]);

                                Serial.print("Behaviour : ");
                                Serial.println(buff[4]);

                                int id = bytesArraytoInt(buff, 2, 1);
                                Serial.print("Id: ");
                                Serial.println(id);

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
    else
    {
        Serial.println("SAP was 0");
        return 0;
    }
}

/*#############################################*/
/*######              TEST 2            #######*/
/*#############################################*/

// Test the connection of one node to the controller and check if a deconnection is asked through SAP to decoonect the device. Indice TEST=2

int test_connexion_deconnexion()
{
    int out = 0;
    Serial.println("Routine de test numéro 2, déconnexion d'une device");
    my_nodes[mydevice_number] = new device();
    mydevice_number++;
    my_nodes[0]->addr = new_addr;

    if (digitalRead(SAP))
    {
        Serial.println("SAP is 1");
        out = send_command(0x08, "ping");
        delay(5);
        Serial.print(" after delay out is : ");
        Serial.println(out);
        if (out)
        {
            Serial.println("Ping was not received at 0x08 meaning a device want deco");
            for (int i = 0; i < mydevice_number; i++)
            {
                Serial.print(" Ping device number :  ");
                Serial.println(i);
                out = send_command(my_nodes[i]->addr, "ping");
                delay(5);
                if (!out)
                {
                    Serial.println(" Ping was received ");
                }
                else
                {
                    Serial.println(" Ping was not received by device i :");
                    return 0;
                }

                if (!digitalRead(SAP))
                {
                    Serial.println(" SAP has been shut down after ping which mean device i is donnected");
                    mydevice_number--;
                    delete my_nodes[i];
                    Serial.println(" device i has been deleted well");
                    break;
                }
                else
                {
                    Serial.println(" SAP was not shut down");
                    return 0;
                }
            }
        }
        else
        {
            Serial.println("ping was received by 0x08 which is not normal");
            return 0;
        }
    }
    else
    {
        Serial.println("SAP was 0");
        return 0;
    }
    return 1;
}

/*#############################################*/
/*######              TEST 3            #######*/
/*#############################################*/

// Test the main function of one node, meaning sending a get_status and sending a publish subject with data about the subject it is subscribed to. TEST indice is 3.
// test bench is a device with a potentiometer, producing 0b00000100 subject and subscribed to 0b00001000.

int test_main_function()
{
    int out = 0;
    Serial.println("Routine de test numéro 3, fonctionnement nominal ");
    int val;
    my_nodes[mydevice_number] = new device();
    mydevice_number++;
    my_nodes[0]->addr = new_addr;
    my_nodes[0]->subscription = 2;
    my_nodes[0]->id=1;
    my_nodes[0]->type= 3;

    my_nodes[0]->my_subjects[0] = new Subject();
    my_nodes[0]->my_subjects[1] = new Subject();
    my_sub_number += 2;

    my_nodes[0]->my_subjects[0]->id = 0b00001000; // Subject that must be sent to potar
    my_nodes[0]->my_subjects[0]->value = 0;

    my_nodes[0]->my_subjects[1]->id = 0b00000010; // Subject that is received by potar
    my_nodes[0]->my_subjects[1]->value = 0;

    uint8_t bytes[4];

    while (!digitalRead(A1))
    {
        delay(500);
        for (int i = 0; i < mydevice_number; i++)
        {
            // boucle pour les status
            out = send_command(my_nodes[i]->addr, "get_status");
            delay(5);
            if (!out)
            {
                Serial.println("get_status was received checking if ACK is 1");
                receive_data(my_nodes[i]->addr, buff, 7);
                if (buff[0] == 1)
                {
                    Serial.println("ACK is 1 after get_status");

                    Serial.print("Device status is : ");
                    Serial.println(buff[1]);

                    Serial.print("Subject produced is of id : ");
                    Serial.println(buff[2]);

                    Serial.print("Subject value is : ");
                    val = bytesArraytoInt(buff, 3, 4);
                    Serial.println(val);

                    for (int k = 0; k < my_sub_number; k++)
                    {
                        if (my_nodes[i]->my_subjects[k]->id == buff[2])
                        {
                            Serial.println("Subject is saved");
                            my_nodes[i]->my_subjects[k]->value = val;
                            break;
                        }
                    }
                }
                else
                {
                    Serial.println("ACK is 0 after get_status");
                    return 0;
                }
            }
            else
            {
                Serial.print("get_status was not received by : ");
                Serial.println(my_nodes[i]->addr);
                return 0;
            }
        }
        for (int i = 0; i < mydevice_number; i++)
        {
            Serial.println("publishing subjects..... ");

            my_subjects[0]->value = !(my_subjects[0]->value);
            intToBytesArray(my_subjects[0]->value, bytes);
            out = send_command(my_nodes[i]->addr, "publish_subject", my_subjects[0]->id, bytes);
            delay(5);
            if (!out)
            {
                Serial.println("publish subject was received checking if ACK is 1");
                receive_data(my_nodes[i]->addr, buff, 1);
                if (buff[0] == 1)
                {
                    Serial.println("ACK is 1 after publish_subject");
                }
                else
                {
                    Serial.println("ACK is 0 after publish subject");
                    return 0;
                }
            }
            else
            {
                Serial.print("publish subject was not received by : ");
                Serial.println(my_nodes[i]->addr);
                return 0;
            }
        }
    }

    return 1;
}

/*#############################################*/
/*######              TEST 0            #######*/
/*#############################################*/

// Test the broadcast function of I2C, in order to send general interests subjects. TEST indice is 0.

int test_broadcast()
{
    int out = 0;

    out = send_command(0x00, "ping");

    if (!out)
    {
        return 1;
    }
    else
    {
        Serial.println(" Ping was not received by broadcast");
        return 0;
    }
}

/*#############################################*/
/*######              TEST 4            #######*/
/*#############################################*/

// Test the connexion of several nodes on the bus at the same time. TEST indice is 4.

int test_multiple_connexion()
{
    return 1;
}

/*#############################################*/
/*######              TEST 5            #######*/
/*#############################################*/

// Test the deconnection of one node on the bus while others are staying in place. TEST indice is 5.

int test_deconnection_real()
{
    return 1;
}

/*#############################################*/
/*######              TEST 6            #######*/
/*#############################################*/

// Test the main function with several devices. They should produce subject and make other device react to it in order to validate the test. TEST indice is 6.

int test_main_multiple_devices()
{
    return 1;
}

/*#############################################*/
/*######              TEST 7            #######*/
/*#############################################*/

// Test the main function of one node and sending bluetooth data to the main node. Referenced as TEST = 7

int test_bluetooth_function_V1()
{
    while (!digitalRead(A1))
    {
        check_ble(my_nodes,mydevice_number);
    }   

    return 1;
}




int test_bluetooth_function()
{
    int out = 0;
    Serial.println("Routine de test numéro 7, fonctionnement nominal et connection bluetooth avec un joystick ");
    int val;
    my_nodes[mydevice_number] = new device();
    mydevice_number++;
    my_nodes[0]->addr = new_addr;
    my_nodes[0]->subscription = 2;
    my_nodes[0]->id=1;
    my_nodes[0]->type= 3;

    my_nodes[0]->my_subjects[0] = new Subject();
    my_nodes[0]->my_subjects[1] = new Subject();
    my_sub_number += 2;

    my_nodes[0]->my_subjects[0]->id = 0b00001000; // Subject that must be sent to potar
    my_nodes[0]->my_subjects[0]->value = 0;

    my_nodes[0]->my_subjects[1]->id = 0b00000010; // Subject that is sent by potar
    my_nodes[0]->my_subjects[1]->value = 0;

    uint8_t bytes[4];

    while (!digitalRead(A1))
    {
        delay(500);
        for (int i = 0; i < mydevice_number; i++)
        {
            // boucle pour les status
            out = send_command(my_nodes[i]->addr, "get_status");
            delay(5);
            if (!out)
            {
                Serial.println("get_status was received checking if ACK is 1");
                receive_data(my_nodes[i]->addr, buff, 7);
                if (buff[0] == 1)
                {
                    Serial.println("ACK is 1 after get_status");

                    Serial.print("Device status is : ");
                    Serial.println(buff[1]);

                    Serial.print("Subject produced is of id : ");
                    Serial.println(buff[2]);

                    Serial.print("Subject value is : ");
                    val = bytesArraytoInt(buff, 4, 3);
                    Serial.println(val);

                    for (int k = 0; k < my_sub_number; k++)
                    {
                        if (my_nodes[i]->my_subjects[k]->id == buff[2])
                        {
                            Serial.println("Subject is saved");
                            my_nodes[i]->my_subjects[k]->value = val;
                            break;
                        }
                    }
                }
                else
                {
                    Serial.println("ACK is 0 after get_status");
                    return 0;
                }
            }
            else
            {
                Serial.print("get_status was not received by : ");
                Serial.println(my_nodes[i]->addr);
                return 0;
            }
        }
        for (int i = 0; i < mydevice_number; i++)
        {
            Serial.println("publishing subjects..... ");

            my_nodes[i]->my_subjects[0]->value = !(my_nodes[i]->my_subjects[0]->value);
            intToBytesArray(my_nodes[i]->my_subjects[0]->value, bytes);
            out = send_command(my_nodes[i]->addr, "publish_subject",my_nodes[i]->my_subjects[0]->id, bytes);
            delay(5);
            if (!out)
            {
                Serial.println("publish subject was received checking if ACK is 1");
                receive_data(my_nodes[i]->addr, buff, 1);
                if (buff[0] == 1)
                {
                    Serial.println("ACK is 1 after publish_subject");
                }
                else
                {
                    Serial.println("ACK is 0 after publish subject");
                    return 0;
                }
            }
            else
            {
                Serial.print("publish subject was not received by : ");
                Serial.println(my_nodes[i]->addr);
                return 0;
            }
        }
        check_ble(my_nodes,mydevice_number);

    }

    return 1;
}
