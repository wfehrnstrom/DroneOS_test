#include <SPI.h>
#include "RF24.h"
//Client Computer is always in transmission role
RF24 radio(7,8); // Set up radio on pins 7 + 8
byte addresses[][2] = {"1", "2"};
void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.setChannel(2);
}

void loop() {
  while(Serial.available() > 0){
    int ledVal = Serial.parseInt();
    if(!radio.write(&ledVal, sizeof(int))){
      Serial.println("Send Failure");
    }
  }
}
