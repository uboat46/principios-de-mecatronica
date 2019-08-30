// CONSTANTS
const int LED_PIN = 13;
const int PHOTO_SENSOR_PIN = A0;
const int POT_SENSOR_PIN = A7;

// SETUP LIGHT LEVELS
int light_value;


// SETUP POTENCIOMETER LEVEL
int pot_value;

// Variable output
int led_output_value;
int pot_percentage_value;
int light_percentage_value;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}


void loop() {
  // Light value at the moment
  light_value = analogRead(PHOTO_SENSOR_PIN);

  // POT value at the moment
  pot_value = analogRead(POT_SENSOR_PIN);

  pot_percentage_value = map(pot_value, 0, 1022, 0, 100);
  light_percentage_value = map(light_value, 0, 1022, 0, 100);
  
  Serial.print("Light value: ");
  Serial.print(light_percentage_value);
  Serial.print(" vs ");
  Serial.print("Pot value: ");
  Serial.print(pot_percentage_value);
  Serial.println("");

  // Compare values
  if ( light_percentage_value > pot_percentage_value) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(100);
}
