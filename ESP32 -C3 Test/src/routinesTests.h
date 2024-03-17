#include <utilities.h>
uint8_t buff[100];

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
            delay(5);
            if (buff[0])
            {
                Serial.println("ACK is 1");
                Serial.println("Creating a new device");

                my_nodes[mydevice_number] = new device();
                mydevice_number++;
                Serial.println("Sending change addr command");

                out = 0;
                delay(10);

                if (!out)
                {
                    Serial.println("Change addr was received checking if ACK is 1");
                    my_nodes[0]->addr = new_addr;
                    // receive_data(new_addr, buff, 1);
                    if (buff[0])
                    {
                        Serial.println("ACK is 1 after change addr");

                        Serial.println("Sending get info command");

                        out = send_command(0x08, "get_info");
                        delay(10);

                        if (!out)
                        {
                            Serial.println("Change addr was received checking if ACK is 1");
                            receive_data(0x08, buff, 5);
                            if (buff[0] == 1)
                            {
                                Serial.print("Subscription : ");
                                Serial.println(buff[3]);

                                Serial.print("Behaviour : ");
                                Serial.println(buff[4]);

                                int id = bytesArraytoInt(buff, 2, 1);
                                Serial.print("Id: ");
                                Serial.println(id);
                            }
                            out = send_command(0x08, "get_info");
                            delay(1000);

                            if (!out)
                            {
                                Serial.println("Change addr was received checking if ACK is 1");
                                receive_data(0x08, buff, 5);
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
                                    return 0;
                                }
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
    my_nodes[0]->id = 1;
    my_nodes[0]->type = 3;

    my_nodes[0]->subjects[0] = new Subject();
    my_nodes[0]->subjects[1] = new Subject();
    my_sub_number += 2;

    my_nodes[0]->subjects[0]->id = 0b00001000; // Subject that must be sent to potar
    my_nodes[0]->subjects[0]->value = 0;

    my_nodes[0]->subjects[1]->id = 0b00000010; // Subject that is received by potar
    my_nodes[0]->subjects[1]->value = 0;

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
                        if (my_nodes[i]->subjects[k]->id == buff[2])
                        {
                            Serial.println("Subject is saved");
                            my_nodes[i]->subjects[k]->value = val;
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
    int out = 0;
    Serial.println("Routine de test numéro 1, connexion d'une device");
    delay(1000);
    while (mydevice_number < 2)
    {
        delay(1000);
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
                    new_addr++;
                    Serial.println("Sending change addr command");

                    out = send_command(0x08, "change_addr", new_addr);
                    delay(10);

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
                            delay(10);

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
                                }
                                else
                                {
                                    Serial.print("ACK is 0 after get_info for : ");
                                    Serial.println(mydevice_number);

                                    return 0;
                                }
                            }
                            else
                            {
                                Serial.print("get_info was not received by : ");
                                Serial.println(mydevice_number);

                                return 0;
                            }
                        }
                        else
                        {
                            Serial.print("ACK is 0 after change addr of : ");
                            Serial.println(mydevice_number);

                            return 0;
                        }
                    }
                    else
                    {
                        Serial.print("change_addr was not received by : ");
                        Serial.println(mydevice_number);

                        return 0;
                    }
                }
                else
                {
                    Serial.print("ACK is 0 for : ");
                    Serial.println(mydevice_number);

                    return 0;
                }
            }
            else
            {
                Serial.print("ping was not received by : ");
                Serial.println(mydevice_number);

                return 0;
            }
        }
        else
        {
            Serial.println("SAP was 0");
        }
    }
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

    // initialise la base de données de noeuds fonctionnels avec les informations de chaques noeuds
    int out = 0;
    Serial.println("Routine de test numéro 6, fonctionnement nominal de deux noeuds fonctionnels et échange d'information permettant la modification du comportement d'un noeud par rapport à un autre");
    int val;

    ///////noeud led/////////////////////////////////
    my_nodes[mydevice_number] = new device();
    mydevice_number++;
    my_nodes[0]->addr = 0x0a;
    my_nodes[0]->subscription = 1;
    my_nodes[0]->id = 1;
    my_nodes[0]->type = 3;

    ///////noeud accelero//////////////////////////////////////
    my_nodes[mydevice_number] = new device();
    mydevice_number++;
    my_nodes[1]->addr = 0x0b;
    my_nodes[1]->subscription = 60;
    my_nodes[1]->id = 1;
    my_nodes[1]->type = 1;

    //////// initialisation des sujets /////////////

    ////// sujet mouv /////
    my_subjects[0] = new Subject();
    my_subjects[0]->id = 1;
    my_subjects[0]->value = 0;
    ////// sujet led_status /////
    my_subjects[1] = new Subject();
    my_subjects[1]->id = 60;
    my_subjects[1]->value = 0;

    my_sub_number = 2;
    /////// stock une référence au pointeur dans les sujets des noeuds
    my_nodes[1]->subjects[0] = my_subjects[0]; // sujet produit par le noeud accelero "mouv"
    my_nodes[1]->subjects[1] = my_subjects[1]; // sujet reçut par le noeud accelero "led_status"
    my_nodes[1]->sub_number = 2;

    my_nodes[0]->subjects[0] = my_subjects[1]; // sujet produit par le noeud led "led_status"
    my_nodes[0]->subjects[1] = my_subjects[0]; // sujet reçut par le noeud led "mouv"
    my_nodes[0]->sub_number = 2;

    /*     Serial.println("test : ");
        Serial.print("subject of id : ");
        Serial.println(my_nodes[0]->subjects[0]->id);
        Serial.print("value is : ");
        Serial.println(my_nodes[0]->subjects[0]->value);
        Serial.print("changing value in subjects list ");
        my_subjects[0]->value = 25;
        Serial.print("subject of id : ");
        Serial.println(my_nodes[0]->subjects[0]->id);
        Serial.print("new value is : ");
        Serial.println(my_nodes[0]->subjects[0]->value);
        Serial.println(my_nodes[1]->subjects[1]->value);   */

    uint8_t bytes[4];

    while (!digitalRead(A1))
    {
        delay(500);
        for (int i = 0; i < mydevice_number; i++)
        {
            if (i == 1)
            {
                // boucle pour les status
                out = send_command(my_nodes[i]->addr, "get_status");
                delay(5);
                if (!out)
                {
                    // on réccupère les status de chaque noeuds

                    Serial.println("get_status was received checking if ACK is 1");
                    receive_data(my_nodes[i]->addr, buff, 7);
                    if (buff[0] == 1)
                    {
                        /*  Serial.println("ACK is 1 after get_status");
                         Serial.print("Node addr is : ");
                         Serial.println(my_nodes[i]->addr);

                         Serial.print("Device status is : ");
                         Serial.println(buff[1]);

                         Serial.print("Subject produced is of id : ");
                         Serial.println(buff[2]);

                         Serial.print("Subject value is : "); */
                        val = bytesArraytoInt(buff, 4, 3);
                        // Serial.println(val);

                        // on les stocks dans nos images des noeuds qu'on a en mémoire pour mettre à jour les valeurs
                        for (int k = 0; k < my_sub_number; k++)
                        {
                            if (my_subjects[k]->id == buff[2])
                            {
                                Serial.println("Subject is saved");
                                my_subjects[k]->value = val;
                                Serial.print("For subject : ");
                                Serial.println(my_subjects[k]->id);
                                Serial.print("value is : ");
                                Serial.println(my_subjects[k]->value);
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
        }
        for (int i = 0; i < mydevice_number; i++)
        {
            if (i == 1)
            {
                Serial.println("publishing subjects..... ");
                intToBytesArray(my_nodes[i]->subjects[1]->value, bytes);
                /* for (int k=0;k<4;k++){
                    Serial.println(bytes[k]);
                } */
                Serial.print("id sent is : ");
                Serial.println(my_nodes[i]->subjects[1]->id);
                Serial.print("to node : ");
                Serial.print(my_nodes[i]->addr);
                out = send_command(my_nodes[i]->addr, "publish_subject", my_nodes[i]->subjects[1]->id, bytes);
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
                        Serial.println(my_nodes[i]->addr);

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
    }
    return 1;
}

/*#############################################*/
/*######              TEST 7            #######*/
/*#############################################*/
#if ENABLE_BLE
// Test the main function of one node and sending bluetooth data to the main node. Referenced as TEST = 7

int test_bluetooth_function_V1()
{
    while (!digitalRead(A1))
    {
        check_ble(my_nodes, mydevice_number);
    }

    return 1;
}

int test_bluetooth_function()
{
    // initialise la base de données de noeuds fonctionnels avec les informations de chaques noeuds
    int out = 0;
    Serial.println("Routine de test numéro 7, fonctionnement nominal de deux noeuds fonctionnels et échange d'information permettant la modification du comportement d'un noeud par rapport à un autre");
    int val;

    ///////noeud led/////////////////////////////////
    my_nodes[mydevice_number] = new device();
    mydevice_number++;
    my_nodes[0]->addr = 0x0a;
    my_nodes[0]->subscription = 1;
    my_nodes[0]->id = 1;
    my_nodes[0]->type = 3;

    ///////noeud accelero//////////////////////////////////////
    my_nodes[mydevice_number] = new device();
    mydevice_number++;
    my_nodes[1]->addr = 0x0b;
    my_nodes[1]->subscription = 60;
    my_nodes[1]->id = 1;
    my_nodes[1]->type = 1;

    //////// initialisation des sujets /////////////

    ////// sujet mouv /////
    my_subjects[0] = new Subject();
    my_subjects[0]->id = 1;
    my_subjects[0]->value = 0;
    ////// sujet led_status /////
    my_subjects[1] = new Subject();
    my_subjects[1]->id = 60;
    my_subjects[1]->value = 0;

    my_sub_number = 2;
    /////// stock une référence au pointeur dans les sujets des noeuds
    my_nodes[1]->subjects[0] = my_subjects[0]; // sujet produit par le noeud accelero "mouv"
    my_nodes[1]->subjects[1] = my_subjects[1]; // sujet reçut par le noeud accelero "led_status"
    my_nodes[1]->sub_number = 2;

    my_nodes[0]->subjects[0] = my_subjects[1]; // sujet produit par le noeud led "led_status"
    my_nodes[0]->subjects[1] = my_subjects[0]; // sujet reçut par le noeud led "mouv"
    my_nodes[0]->sub_number = 2;

    uint8_t bytes[4];

    while (!digitalRead(A1))
    {
        delay(500);
        for (int i = 0; i < mydevice_number; i++)
        {
            if (i == 1)
            {
                // boucle pour les status
                out = send_command(my_nodes[i]->addr, "get_status");
                delay(5);
                if (!out)
                {
                    // on réccupère les status de chaque noeuds

                    Serial.println("get_status was received checking if ACK is 1");
                    receive_data(my_nodes[i]->addr, buff, 7);
                    if (buff[0] == 1)
                    {
                        /*  Serial.println("ACK is 1 after get_status");
                         Serial.print("Node addr is : ");
                         Serial.println(my_nodes[i]->addr);

                         Serial.print("Device status is : ");
                         Serial.println(buff[1]);

                         Serial.print("Subject produced is of id : ");
                         Serial.println(buff[2]);

                         Serial.print("Subject value is : "); */
                        val = bytesArraytoInt(buff, 4, 3);
                        // Serial.println(val);

                        // on les stocks dans nos images des noeuds qu'on a en mémoire pour mettre à jour les valeurs
                        for (int k = 0; k < my_sub_number; k++)
                        {
                            if (my_subjects[k]->id == buff[2])
                            {
                                Serial.println("Subject is saved");
                                my_subjects[k]->value = val;
                                Serial.print("For subject : ");
                                Serial.println(my_subjects[k]->id);
                                Serial.print("value is : ");
                                Serial.println(my_subjects[k]->value);
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
        }
        for (int i = 0; i < mydevice_number; i++)
        {
            if (i == 1)
            {
                Serial.println("publishing subjects..... ");
                intToBytesArray(my_nodes[i]->subjects[1]->value, bytes);
                /* for (int k=0;k<4;k++){
                    Serial.println(bytes[k]);
                } */
                Serial.print("id sent is : ");
                Serial.println(my_nodes[i]->subjects[1]->id);
                Serial.print("to node : ");
                Serial.print(my_nodes[i]->addr);
                out = send_command(my_nodes[i]->addr, "publish_subject", my_nodes[i]->subjects[1]->id, bytes);
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
                        Serial.println(my_nodes[i]->addr);

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
        check_ble(my_nodes, mydevice_number);
    }
    return 1;
}
#else
int test_bluetooth_function()
{
    return 1;
}
#endif
/*#############################################*/
/*######              TEST 8            #######*/
/*#############################################*/

// Test the main function of one node and sending bluetooth data to the main node with several nodes connected. Referenced as TEST = 8

int test_esp32_slave_pinout()
{
    // initialise la base de données de noeuds fonctionnels avec les informations de chaques noeuds
    int out = 0;
    Serial.println("Routine de test numéro 6, fonctionnement nominal de deux noeuds fonctionnels et échange d'information permettant la modification du comportement d'un noeud par rapport à un autre");
    int val;

    ///////noeud led/////////////////////////////////
    my_nodes[mydevice_number] = new device();
    mydevice_number++;
    my_nodes[0]->addr = 0x08;

    uint8_t bytes[4];

    while (!digitalRead(A1))
    {
        Serial.print("SAP value is : ");
        Serial.println(digitalRead(A1));

        delay(1000);
        for (int i = 0; i < mydevice_number; i++)
        {
            // boucle pour les status
            out = send_command(0x08, "ping");
            delay(5);
            if (!out)
            {
                // on réccupère les status de chaque noeuds

                Serial.println("ping was received checking if ACK is 1");
                receive_data(my_nodes[i]->addr, buff, 1);
                if (buff[0] == 1)
                {
                    Serial.println("ping received by esp32");
                }
                else
                {
                    Serial.println("ACK is 0 after ping");
                    return 0;
                }
            }
            else
            {
                Serial.print("ping was not received by : ");
                Serial.println(my_nodes[i]->addr);
                return 0;
            }
        }
    }
    return 1;
}

/*#############################################*/
/*######              TEST 9            #######*/
/*#############################################*/

// Test pour l'esp32 en slave et le bus i2c qui envoi nimp au premier ping

int test_i2c_esp32()
{
    int out = 0;
    Serial.println("Routine de test numéro 1, connexion d'une device");
    Serial.println("SAP is 1");
    for (int k = 0; k < 2; k++)
    {
        out = send_command(0x08, "ping");
        delay(5);
        Serial.print(" after delay out is : ");
        Serial.println(out);
        if (!out)
        {
            Serial.println("Ping has been received at 0x08 apearing the new device if ack");
            delay(5);
            receive_data(0x08, buff, 1);

            Serial.print("data received after ping number : ");
            Serial.println(k);
            Serial.print("ack value is : ");
            Serial.println(buff[0]);
        }
        else
        {
            Serial.println("Ping wa not received at 0x08 ");

            return 0;
        }
    }

    return 1;
}