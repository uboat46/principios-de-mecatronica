#include <SoftwareSerial.h>
SoftwareSerial XBee = SoftwareSerial(0,1);

void setup() {
  Serial.begin(9600);
  pinMode(0, INPUT);
  pinMode(1, OUTPUT);
  XBee.begin(9600);
}

void loop() {
  Serial.write('D');
  if(XBee.available() > 0) {
    int dato = XBee.read();
    Serial.write(char(dato));
  }
  delay(1000);
}
