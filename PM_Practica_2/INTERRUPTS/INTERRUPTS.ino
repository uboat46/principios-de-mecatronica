// led pin
int led_pin = 13;

// interrupt pin
int interrupt_pin = 2;

// counter
float counter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  pinMode(interrupt_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), count, FALLING );
}

void loop() {
  asm (
    "inicio: \n\t"
    "sbi 0x05, 7 \n\t"
    
    "call tiempo \n\t"
    "call tiempo \n\t"
    "call tiempo \n\t"
    "call tiempo \n\t"

    "cbi 0x05, 7 \n\t"

    "call tiempo \n\t"
    "call tiempo \n\t"
    "call tiempo \n\t"
    "call tiempo \n\t"
    
    "jmp inicio \n\t"
    
    "tiempo: \n\t"
    "LDI r22, 40 \n\t"
    "LOOP_3: \n\t"
    "LDI r21, 255 \n\t"
    "LOOP_2: \n\t"
    "LDI r20, 255 \n\t"
    "LOOP_1: \n\t"
    "DEC r20 \n\t"
    "BRNE LOOP_1 \n\t"
    "DEC r21 \n\t"
    "BRNE LOOP_2 \n\t"
    "DEC r22 \n\t"
    "BRNE LOOP_3 \n\t"
    "ret \n\t"
  );
}

void count() {
  counter = counter + 1;
  Serial.println(counter);
}