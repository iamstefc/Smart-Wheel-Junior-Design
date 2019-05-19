#include "DRV8833.h"
DRV8833 driver = DRV8833();

const int inputA1 = 5, inputA2 = 6, inputB1 = 9, inputB2 = 10;
const int motorSpeed = 128; // Half speed (255 / 2).
void setup()
{
  Serial.begin(115200);
  while(!Serial);  
// Attach the motors to the input pins:
  pinMode(inputA1, OUTPUT);
    pinMode(inputA2, OUTPUT);
      pinMode(inputB1, OUTPUT);
    pinMode(inputB2, OUTPUT);
}

void loop()
{
  Serial.println("45:");
    digitalWrite(inputA1, LOW);
    digitalWrite(inputA2, LOW);
   delay(500);

   // Serial.println("4ft:");
    digitalWrite(inputB1, LOW);
    digitalWrite(inputB2, LOW);
  delay(500);
}

