// LED pins
int led_pin_1 = 13;
int led_pin_2 = 12;
int led_pin_3 = 11;
int led_pin_4 = 10;

// LED intervals
const long led_interval_1 = 2000;
const long led_interval_2 = 1000;
const long led_interval_3 = 500;
const long led_interval_4 = 250;

// LED states
int led_state_1 = LOW;
int led_state_2 = LOW;
int led_state_3 = LOW;
int led_state_4 = LOW;

// LED milis
unsigned long led_1_timer = 0;
unsigned long led_2_timer = 0;
unsigned long led_3_timer = 0;
unsigned long led_4_timer = 0;

// current millis time
unsigned long currentMillis;

void setup() {
  pinMode(led_pin_1, OUTPUT);
  pinMode(led_pin_2, OUTPUT);
  pinMode(led_pin_3, OUTPUT);
  pinMode(led_pin_4, OUTPUT);
}

void loop() {
  currentMillis = millis();

  if (currentMillis - led_1_timer >= led_interval_1) {
    if (led_state_1 == LOW) {
      led_state_1 = HIGH;
    } else {
      led_state_1 = LOW;
    }

    led_1_timer = currentMillis;
    digitalWrite(led_pin_1, led_state_1);
  }

  if (currentMillis - led_2_timer >= led_interval_2) {
    if (led_state_2 == LOW) {
      led_state_2 = HIGH;
    } else {
      led_state_2 = LOW;
    }

    led_2_timer = currentMillis;
    digitalWrite(led_pin_2, led_state_2);
  }
  
  if (currentMillis - led_3_timer >= led_interval_3) {
    if (led_state_3 == LOW) {
      led_state_3 = HIGH;
    } else {
      led_state_3 = LOW;
    }

    led_3_timer = currentMillis;
    digitalWrite(led_pin_3, led_state_3);
  }
  
  if (currentMillis - led_4_timer >= led_interval_4) {
    if (led_state_4 == LOW) {
      led_state_4 = HIGH;
    } else {
      led_state_4 = LOW;
    }

    led_4_timer = currentMillis;
    digitalWrite(led_pin_4, led_state_4);
  }

}
