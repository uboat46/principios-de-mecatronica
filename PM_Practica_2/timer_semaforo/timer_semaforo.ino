int sec = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  cli();
  TCCR1B = 0; TCCR1A = 0;
  TCCR1B |= (1 << CS12);
  TCNT1 = 3036;
  TIMSK1 |= (1 << TOIE1);
  sei();
}

void loop() {
  if (sec == 1) {
    digitalWrite(2, HIGH);
    digitalWrite(5, HIGH);
  }
  if (sec == 5){
    digitalWrite(3, HIGH);
    digitalWrite(2, LOW);
  }
  if (sec == 6){
    digitalWrite(4, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(5, LOW);
  }
  if (sec == 11){
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
  }
  if (sec == 12){
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(4, LOW);
  }
  if (sec == 13){
    digitalWrite(2, HIGH);
    digitalWrite(5, HIGH);
    sec = 0;
  }
}


ISR(TIMER1_OVF_vect)
{
  sec = sec + 1;
}
