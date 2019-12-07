#include <PID_v1.h>
#include <math.h>

#define PI 3.1415926535897932384626433832795

const byte interruptPinDer = 3;
const byte interruptPinIzq = 2;
volatile byte state = LOW;

unsigned long tiempoVueltaDer = 0;
double contadorDer = 0;
int inicioDer = 0;

unsigned long tiempoVueltaIzq = 0;
double contadorIzq = 0;
int inicioIzq = 0;

double velocidadAngularDer = 0;
double velocidadAngularIzq = 0;


//Define Variables we'll be connecting to
double velocidadNormaDer, entregaVelocidadDer;
double velocidadNormaIzq, entregaVelocidadIzq;

//Specify the links and initial tuning parameters
PID myPIDDer(&velocidadAngularDer, &entregaVelocidadDer, &velocidadNormaDer, 2, 5, 1, P_ON_E, DIRECT); //P_ON_M specifies that Proportional on Measurement be used
PID myPIDIzq(&velocidadAngularIzq, &entregaVelocidadIzq, &velocidadNormaIzq, 2, 5, 1, P_ON_E, DIRECT); //P_ON_M specifies that Proportional on Measurement be used


void setup() {

  // Set timer to count wheels rmp every 100ms
  cli();
  TCCR1B = 0;
  TCCR1A = 0;
  TCNT1 = 0;
  TCCR1B |= (1 << CS12);
  // set compare match register for 10hz increments
  OCR1A = 6252;
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();
  
  Serial.begin(9600);
  pinMode(interruptPinDer, INPUT_PULLUP);
  pinMode(interruptPinIzq, INPUT_PULLUP);

  pinMode(9, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  
  // Set initial rotation direction DER
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  // Set initial rotation direction IZQ
  digitalWrite(5, HIGH);
  digitalWrite(4, LOW);

  velocidadNormaDer = 12;
  velocidadNormaIzq = 12;
  
  //turn the PID on
  myPIDDer.SetMode(AUTOMATIC);
  myPIDIzq.SetMode(AUTOMATIC);
  myPIDDer.SetSampleTime(100);
  myPIDIzq.SetSampleTime(100);

  attachInterrupt(digitalPinToInterrupt(interruptPinDer), cuentaRPMDer, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPinIzq), cuentaRPMIzq, RISING);
}

void loop() {

//    if (interruptPinDer) {
//      contadorDer = contadorDer + 1;
//    }
//
//    if (interruptPinIzq) {
//      contadorIzq = contadorIzq + 1;
//    }
    
    myPIDDer.Compute();
    myPIDIzq.Compute();
    
    analogWrite(8, entregaVelocidadIzq);
    analogWrite(9, entregaVelocidadDer);

//  Serial.print(entregaVelocidadIzq);
//  Serial.print(" ");
//  Serial.println(entregaVelocidadDer);
//  Serial.print(" ");
  Serial.print(velocidadAngularIzq);
  Serial.print(" ");
  Serial.println(velocidadAngularDer);
//    Serial.print(contadorIzq);
//    Serial.print(" ");
//    Serial.println(contadorDer);

}

void cuentaRPMDer() {
  contadorDer = contadorDer + 1;
}

void cuentaRPMIzq() {  
  contadorIzq = contadorIzq + 1;
}


ISR(TIMER1_COMPA_vect){//timer1 interrupt 10Hz = 100ms  => 16,000,000 / {(prescaler * desired hz) - 1}
  cli();
    contadorDer = (contadorDer / 20) / 0.1; // vuealtas dadas/segundo
    contadorIzq = (contadorIzq / 20) / 0.1; // vuealtas dadas/segundo
            
    velocidadAngularDer = 2 * PI * contadorDer; // velocidad angular en rad/s
    velocidadAngularIzq = 2 * PI * contadorIzq; // velocidad angular en rad/s

    contadorIzq = 0;
    contadorDer = 0;
  sei();
}


//if (inicioIzq == 0) {
//    inicioIzq = 1;
//    tiempoVueltaIzq = millis();
//  } else {
//    contadorIzq = millis();
//    tiempoVueltaIzq = contadorIzq - tiempoVueltaIzq;
//    tiempoVueltaIzq = tiempoVueltaIzq * 20; // tiempo en dar una vuelta completa en ms
//    velocidadAngularIzq = (2000 * PI) / tiempoVueltaIzq; // velocidad angular en rad/s
//    inicioIzq = 0;
//  }
//
//  if(isinf(velocidadAngularDer) || velocidadAngularDer > 255) {
//    velocidadAngularDer = 255;
//  }
