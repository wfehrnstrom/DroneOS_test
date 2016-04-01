#include <SPI.h>
#include "RF24.h"

RF24 radio(7, 8);
int ledVal = 0;
byte addresses[][2] = {"1", "2"};
void setup() {
  pinMode(3, OUTPUT);
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.openReadingPipe(1, addresses[1]);
  radio.setChannel(2);
  radio.startListening();
}
       
void loop() {
  radio.read(&ledVal, sizeof(int)); 
  analogWrite(3, ledVal);
}
