
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

byte addresses[][6] = {"Node1","Node2"};

// Used to control whether this node is sending or receiving
bool role = 1;
bool over = false;
unsigned long numSent = 1000;
unsigned long transmissionCount = 0;
unsigned long numFailed = 0;
unsigned long numSuccessful = 0;
unsigned long dataRateSum = 0; 
unsigned long disconnectStart = 1;
unsigned long disconnectEnd = 0;
unsigned long bottleneckThreshold = 1000;
int numBottlenecks = 0;
void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_MIN);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  radio.setChannel(2);
  // Start the radio listening for data
  radio.startListening();
}

void loop() {
  if(transmissionCount < numSent){
  
/****************** Ping Out Role ***************************/  
if (role == 1)  {
    
    radio.stopListening();                                    // First, stop listening so we can talk.
    
    
//    Serial.println(F("Now sending"));

    unsigned long start_time = micros();                             // Take the time, and send it.  This will block until complete
    boolean sendStatus = radio.write( &transmissionCount, sizeof(unsigned long) );
//     if (!sendStatus){
//       Serial.println(F("failed"));
//     } else {
//       Serial.println(F("NOT failed"));
//     }
        
    transmissionCount++;
    radio.startListening();                                    // Now, continue listening
    
    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 2000 ){            // If waited longer than 2ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){       // Describe the results
        disconnectStart = micros();
        Serial.println(F("Failed, response timed out."));
        numFailed++;
    }else{
        if(disconnectStart != 1){
          disconnectEnd = micros();
          unsigned long bottleneckTime = disconnectEnd - disconnectStart;
          if(bottleneckTime > bottleneckThreshold){
            numBottlenecks++;
          }          
          disconnectStart = 1;
        }
        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
        radio.read( &got_time, sizeof(unsigned long) );
        unsigned long end_time = micros();
        
        // Spew it
        Serial.print(F("Sent "));
        Serial.print(start_time);
        Serial.print(F(", Got response "));
        Serial.print(got_time);
        Serial.print(F(", Round-trip delay "));
        unsigned timeElapsed = end_time-start_time;
        dataRateSum += timeElapsed;
        Serial.print(timeElapsed);
        Serial.println(F(" microseconds"));
        numSuccessful++;
      }
  
    delay(50);
   }
  }
  else if(!over){
  double avgRate = ((double)dataRateSum/numSuccessful);
  double percentSuccess = ((float)numSuccessful/numSent) * 100;
  double percentFailure = ((float)numFailed/numSent) * 100;
  Serial.print("Percent success out of: ");
  Serial.print(numSent);
  Serial.print(" is: ");
  Serial.print(percentSuccess);
  Serial.println();
  Serial.print("Percent Failure out of: ");
  Serial.print(numSent);
  Serial.print(" is: ");
  Serial.print(percentFailure);
  Serial.println();
  Serial.print("Average data rate: ");
  Serial.print(avgRate);
  Serial.println(F(" microseconds"));
  Serial.println();
  Serial.print("Number of disconnects: ");
  Serial.print(numBottlenecks);
  Serial.println();
  over = true;
  }
} // Loop

