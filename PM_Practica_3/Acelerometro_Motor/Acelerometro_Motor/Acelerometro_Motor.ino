// led pin
#include <Wire.h>

int speedPin = 3;
int motorLPin = 6;
int motorRPin = 7;
int speed_value_motor;
int velocidad;

int estado1 = 0;
int estado2 = 0;


void setup() {
  Serial.begin(9600);
  pinMode(speedPin, OUTPUT);
  pinMode(motorLPin, OUTPUT);
  pinMode(motorRPin, OUTPUT);
  //Wire.begin(1);
  //Wire.onReceive(receiveEvent);
  speed_value_motor = 0;
  // pot_percentage_value = map(pot_value, 0, 1022, 0, 100);  
}

void loop() {
  analogWrite(speedPin, velocidad);
  //Serial.print(velocidad);
  //Serial.println(speed_value_motor);
  if (Serial.available() > 0) {
    
    int dato = Serial.read();
    Serial.println(char(dato));
    if (char(dato) == '0') {
      speed_value_motor = speed_value_motor + 1;  
    } else if (char(dato) == '2'){
      speed_value_motor = speed_value_motor - 1;
    }
    
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
}


void receiveEvent(int howMany) {
  //Serial.print("Hello");
  if(Wire.available() > 0) {
    int dato = Wire.read();
    Serial.println(dato);
    if (dato == 255) {
       speed_value_motor = speed_value_motor + 1;
    } else if (dato == 1) {
      speed_value_motor = speed_value_motor - 1;
    }
  }

  //Serial.print("Velocidad: ");
  //Serial.println(speed_value_motor);

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
