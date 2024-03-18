#include <Controller.h>

uint8_t buff[30];

uint8_t data[5];

uint8_t new_addr = 0x09;

Device *my_nodes[MAX_DEVICE]; // device storage
uint8_t my_nodes_number = 0;

Subject *my_subjects[30];
uint8_t my_sub_number = 0;

uint8_t handle_connexion(){

return 0;
}; 
uint8_t handle_deconnexion(){
    int out=0;
    int addr_deco=1;
    for (int i = 0; i < my_nodes_number; i++)
            {
                Serial.print(" Ping device number :  ");
                Serial.println(i);
                out = send_command(my_nodes[i]->addr, "ping");
                delay(5);
                if (!out)
                {
                    Serial.println(" Ping was received ");
                }
                if (!digitalRead(SAP))
                {
                    Serial.println(" SAP has been shut down after ping which mean device i is donnected");
                    my_nodes_number--;
                    addr_deco=my_nodes[i]->addr;
                    delete my_nodes[i];
                    Serial.print(" device ");
                    Serial.print(addr_deco);
                    Serial.println( " has been deleted well");
                    break;
                }
            }
    return addr_deco;
};

uint8_t get_status(){
    int out=0;
        uint8_t bytes[4];
        int val;
for (int i = 0; i < my_nodes_number; i++)
        {
            // boucle pour les status
            Serial.println(i);
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
return 0;

};
uint8_t distribute_subject(){
int out=0;
        uint8_t bytes[4];

 for (int i = 0; i < my_nodes_number; i++)
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
return 0;

};

uint8_t controller_do_control(){
    uint8_t catch_error = 0;
    //checking if a node need connexion or deconnexion approval
    int SAP_value = digitalRead(SAP);
    if(SAP_value){
        catch_error=handle_connexion(); //ping 0x08 checking if a new device is connected 
        if (catch_error==1){
            catch_error = handle_deconnexion(); // if 0x08 doesn't respond it means that a device ask to deconnect 
            if (catch_error==1){
                Serial.println("Nobody respond to deconnexion");
            }
            else if (catch_error==0){
                Serial.println("Error occured during deconnexion process");
            }
        }
        else if (catch_error==0){
            Serial.println("Error occured during connexion process");
        }
    }

    // Main function of controller, get status and distribute subjects to the right node 

    catch_error=get_status();
    if (catch_error==0){
        Serial.println("Error occured during get_status process");
    }

    catch_error=distribute_subject();

        if (catch_error==0){
        Serial.println("Error occured during subject distribution");
    }

    check_ble(my_nodes,my_nodes_number);

return 1;
};
