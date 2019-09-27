// counter
float counter = 0;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
//  cli();
//  TCCR1B = 0; TCCR1A = 0;
//  TCCR1B |= (1 << CS12);
//  TCNT1 = 3036;
//  TIMSK1 |= (1 << TOIE1);
//  sei();
}

void loop() {
  PORTB = PORTB ^ B10000000;
  delay(100);
//  asm (
//    "inicio: \n\t"
//    "sbi 0x05, 7 \n\t"
//    
//    "call tiempo \n\t"
//    "call tiempo \n\t"
//    "call tiempo \n\t"
//    "call tiempo \n\t"
//
//    "cbi 0x05, 7 \n\t"
//
//    "call tiempo \n\t"
//    "call tiempo \n\t"
//    "call tiempo \n\t"
//    "call tiempo \n\t"
//    
//    "jmp inicio \n\t"
//    
//    "tiempo: \n\t"
//    "LDI r22, 40 \n\t"
//    "LOOP_3: \n\t"
//    "LDI r21, 255 \n\t"
//    "LOOP_2: \n\t"
//    "LDI r20, 255 \n\t"
//    "LOOP_1: \n\t"
//    "DEC r20 \n\t"
//    "BRNE LOOP_1 \n\t"
//    "DEC r21 \n\t"
//    "BRNE LOOP_2 \n\t"
//    "DEC r22 \n\t"
//    "BRNE LOOP_3 \n\t"
//    "ret \n\t"
//  );
}

ISR(TIMER1_OVF_vect)
{
  counter = counter + 1;
  PORTB = PORTB ^ B10000000;
  Serial.println(counter);
}
