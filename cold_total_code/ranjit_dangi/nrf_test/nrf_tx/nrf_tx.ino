#include <SPI.h>  
#include "RF24.h"
#include "nRF24L01.h"
RF24 myRadio (48, 49);
byte addresses[][6] = {"0001"};

struct package
{
  int id=1;
  int temperature = 18.3;
  char  text[100] = "mutech";
};


typedef struct package Package;
Package data;


void setup()
{
  Serial.begin(115200);
  delay(1000);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_2MBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);
  delay(1500);
}

void loop()
{
  myRadio.write(&data, sizeof(data)); 

  Serial.print("\nPackage:");
  Serial.print(data.id);
  Serial.print("\n");
  Serial.println(data.temperature);
  Serial.println(data.text);
  data.id = data.id + 1;
  data.temperature = data.temperature+0.1;
  delay(1000);

}
