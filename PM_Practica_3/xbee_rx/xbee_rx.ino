#include <XBee.h>
#define MAX_FRAME_DATA_SIZE 110
XBee xbee = XBee();

char rx;


void setup() {
  pinMode(0, INPUT);
  pinMode(1, OUTPUT);
  pinMode(13, OUTPUT);
 Serial.begin(9600);
 xbee.setSerial(Serial);
}

void loop() {
  if (Serial.available()>0){
    rx = Serial.read();
    digitalWrite(13, HIGH);
  }
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  //Serial.print("L");
  //delay(100);
}
