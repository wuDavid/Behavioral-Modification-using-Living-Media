#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);

const uint64_t pipe = 0xE8E8F0F0E1LL;

int StepCnt; // declare StepCnt integer
//String voltageVal;
//String message;
int voltageVal;
//char sentvalues[16];

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(pipe);

  //pinMode (1, INPUT); //pushbutton on/off

  //digitalWrite (1, LOW); //starts off
}

// the loop routine runs over and over again forever:
void loop() {
  int sensorValue = analogRead(A0); // read the input on analog pin 0
  float voltage = sensorValue * (5.0 / 1023.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  if (voltage < 1){
    StepCnt++;
  }
  // figure out if you have a step
  // if (step) { StepCnt++; }
  
  // if we can successfully connect with the base station (and number of steps meets some minimum)
  // transmit number of steps
  // StepCnt = 0;
  
  char SendPayload[31] = "";
  voltageVal = "0";
  char tmp[4];
  dtostrf(voltage, 4, 2, tmp);
  voltageVal += tmp;
  message = "S" + voltageVal + "L";
  int charToSend[16];
  int messageSize = message.length();
    for (int i = 0; i<messageSize; i++){
      SendPayload[i] = message.charAt(i);
    }
    unsigned long time = millis();
  bool ok = radio.write(&SendPayload, strlen(SendPayload));
  delay (50);
}

