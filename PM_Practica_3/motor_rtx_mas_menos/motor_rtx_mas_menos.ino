#include <Wire.h>

int speedPin = 3;
int motorLPin = 6;
int motorRPin = 7;
int speed_value_motor;
int velocidad;

int estado1 = 0;
int estado2 = 0;

void setup() {
  pinMode(speedPin, OUTPUT);
  pinMode(motorLPin, OUTPUT);
  pinMode(motorRPin, OUTPUT);
  Wire.begin(1);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  speed_value_motor = 0;
}

void loop() {
  analogWrite(speedPin, velocidad);
}

void receiveEvent(int howMany) {
  if(Wire.available() == 2) {
    estado2 = Wire.read();
    if (estado2 == 1) {
       speed_value_motor = speed_value_motor + 1;
    }
  }

  if(Wire.available() == 1) {
    estado1 = Wire.read();
    if (estado1 == 1) {
      speed_value_motor = speed_value_motor - 1;
    }
  }

  Serial.print("Velocidad: ");
  Serial.println(speed_value_motor);

  if (speed_value_motor < 0) {
    digitalWrite(motorLPin, HIGH);
    digitalWrite(motorRPin, LOW);
    velocidad = speed_value_motor * -1;
  } else {
    digitalWrite(motorLPin, LOW);
    digitalWrite(motorRPin, HIGH);
    velocidad = speed_value_motor;
  }
  
    
}

