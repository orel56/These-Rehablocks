#include <routinesTests.h>

// TEST functions are wrote in routinesTests.h file and are selected using TEST define in config.h.

int (*test)() = nullptr;

int end = -1;

unsigned long begin_time = 0;

unsigned long end_time = 0;

unsigned long spent_time = 0;

void handle(){};

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Wire.setTimeOut(100);
  pinMode(SAP, INPUT_PULLDOWN);

  //init_ble();
  
#ifdef TEST
  if (TEST == 0)
  {
    test = &test_broadcast;
  }
  else if (TEST == 1)
  {
    test = &test_connexion;
  }
  else if (TEST == 2)
  {
    test = &test_connexion_deconnexion;
  }
  else if (TEST == 3)
  {
    test = &test_main_function;
  }
  else if (TEST== 4){
    test = &test_multiple_connexion;
  }
  else if (TEST == 7)
  {
    test = &test_bluetooth_function;
  }
#else
  Serial.println("TEST is not define");
  test = &(handle);
#endif
}

void loop()
{

  delay(10000);
  if (end == -1)
  {
    begin_time = millis();
    end = (*test)();
    end_time = millis();

    spent_time = end_time - begin_time;

    Serial.print("test ended in : ");
    Serial.print(spent_time);
    Serial.println(" ms");
    Serial.print("It went : ");

    if (!end)
    {
      Serial.println("bad");
    }
    else
    {
      Serial.println("good");
    }
    Serial.println("######################################################");
  }
}
