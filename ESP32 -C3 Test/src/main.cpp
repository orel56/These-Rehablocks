#include <routinesTests.h>

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
#ifdef TEST
  if (TEST == 1)
  {
    test = &test_connexion;
  }
  else if (TEST == 2)
  {
    test = &test_connexion_deconnexion;
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
