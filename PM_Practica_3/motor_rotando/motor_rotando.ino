#include <Wire.h>

int speedPin = 3;
int motorLPin = 6;
int motorRPin = 7;
int speed_value_motor;

int estado1 = 0;
int estado2 = 0;

void setup() {
  pinMode(speedPin, OUTPUT);
  pinMode(motorLPin, OUTPUT);
  pinMode(motorRPin, OUTPUT);
  Wire.begin(1);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  speed_value_motor = 125;
}

void loop() {
  analogWrite(speedPin, speed_value_motor);
}

void receiveEvent(int howMany) {
  if(Wire.available() == 2) {
    estado2 = Wire.read();
  }

  if(Wire.available() == 1) {
    estado1 = Wire.read();
  }
  
  if (estado1 == 0 && estado2 == 0) {
    digitalWrite(motorLPin, LOW);
    digitalWrite(motorRPin, LOW);
  }

  if (estado1 == 0 && estado2 == 1) {
    digitalWrite(motorLPin, HIGH);
    digitalWrite(motorRPin, LOW);
  }

  if (estado1 == 1 && estado2 == 0) {
    digitalWrite(motorLPin, LOW);
    digitalWrite(motorRPin, HIGH);
  }

  if (estado1 == 1 && estado2 == 1) {
    digitalWrite(motorLPin, HIGH);
    digitalWrite(motorRPin, HIGH);
  }
    
}

