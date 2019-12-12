#include <PID_v1.h>
#include <math.h>

//  ========= CONSTANTS =========
#define PI 3.1415926535897932384626433832795
#define DIAMETRO 6.6
#define L_RUEDAS 13.5
#define VELOCIDAD 10 // cm/s

//  ========= INTERRUPT PINS FOR THE WHEELS =========
const byte interruptPinDer = 3;
const byte interruptPinIzq = 2;

//  ========= INTERRUPT COUNTERS =========
volatile unsigned contadorDer = 0;
volatile unsigned contadorIzq = 0;

//  ========= ODOMETRY =========
double velocidadAngularDer = 0;
double velocidadLinearDer = 0;
double velocidadAngularIzq = 0;
double velocidadLinearIzq = 0;


float distancia = 0;
float x = 0;
float y = 0;
float phi = 0;


//  ========= CONTROL =========
double phiDeseado = 0;
double xDeseado = 20;
double yDeseado = 0;
double error = 0;
double W = 0;


//  ========= PID VARIABLES =========
double velocidadNormaDer, entregaVelocidadDer;
double velocidadNormaIzq, entregaVelocidadIzq;

//Specify the links and initial tuning parameters
PID myPIDDer(&velocidadAngularDer, &entregaVelocidadDer, &velocidadNormaDer, 2, 5, 1, P_ON_M, DIRECT); //P_ON_M specifies that Proportional on Measurement be used
PID myPIDIzq(&velocidadAngularIzq, &entregaVelocidadIzq, &velocidadNormaIzq, 2, 5, 1, P_ON_M, DIRECT); //P_ON_M specifies that Proportional on Measurement be used


void setup() {
  
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

  velocidadNormaDer = 0;
  velocidadNormaIzq = 0;
  
  //turn the PID on
  myPIDDer.SetMode(AUTOMATIC);
  myPIDIzq.SetMode(AUTOMATIC);
  myPIDDer.SetSampleTime(100);
  myPIDIzq.SetSampleTime(100);

  attachInterrupt(digitalPinToInterrupt(interruptPinDer), cuentaRPMDer, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPinIzq), cuentaRPMIzq, RISING);
}

void loop() {

    phiDeseado = atan2(yDeseado - y, xDeseado - x);

    error = phiDeseado - phi;
    W = ((velocidadLinearDer - velocidadLinearIzq) / L_RUEDAS) + (10 * error);
    
    velocidadNormaDer = VELOCIDAD + (W * L_RUEDAS) / 2;
    velocidadNormaIzq = VELOCIDAD - (W * L_RUEDAS) / 2;
    
    myPIDDer.Compute();
    myPIDIzq.Compute();
    
    if (abs(x - xDeseado) < 5 && abs(y - yDeseado) < 5) { // ya estamos a 5 cm de la meta mejor paremonos
      analogWrite(8, 0);
      analogWrite(9, 0);
//      Serial.println("LLEGUE");
    } else {
      analogWrite(8, entregaVelocidadIzq);
      analogWrite(9, entregaVelocidadDer);
//      Serial.println("AHI VOY");
    }

//    Serial.print(entregaVelocidadIzq);
//    Serial.print(" ");
//    Serial.println(entregaVelocidadDer);
//    Serial.print(" ");
//    Serial.print(velocidadAngularIzq);
//    Serial.print(" ");
//    Serial.println(velocidadAngularDer);
//    Serial.print(contadorIzq);
//    Serial.print(" ");
//    Serial.println(contadorDer);
//    Serial.print(x);
//    Serial.print(" ");
//    Serial.println(y);
}

void cuentaRPMDer() {
  contadorDer = contadorDer + 1;
}

void cuentaRPMIzq() {  
  contadorIzq = contadorIzq + 1;
}


ISR(TIMER1_COMPA_vect){//timer1 interrupt 10Hz = 100ms  => 16,000,000 / {(prescaler * desired hz) - 1}
  cli();
    double vueltasDerSegundos = (((double) contadorDer) / 20.00) / 0.1; // vuealtas dadas/segundo
    double vueltasIzqSegundos = (((double) contadorIzq) / 20.00) / 0.1; // vuealtas dadas/segundo
            
    velocidadAngularDer = 2 * PI * vueltasDerSegundos; // velocidad angular en rad/s
    velocidadLinearDer = (DIAMETRO / 2) * velocidadAngularDer; // velocidad llanta der en cm/s
    
    velocidadAngularIzq = 2 * PI * vueltasIzqSegundos; // velocidad angular en rad/s
    velocidadLinearIzq = (DIAMETRO / 2) * velocidadAngularIzq; // velocidad llanta izq en cm/s

    double distanciaDer = PI * DIAMETRO * ((double) contadorDer / 20.00 );
    double distanciaIzq = PI * DIAMETRO * ((double) contadorIzq / 20.00 );

    distancia = (distanciaDer + distanciaIzq) / 2;

    Serial.print(x);
    Serial.print(" ");
    Serial.println(y);
    x = x + (distancia * cos(phi));
    y = y + (distancia * sin(phi));
    phi = phi + ((distanciaDer - distanciaIzq) / L_RUEDAS);
    phi = atan2(sin(phi), cos(phi));

    contadorIzq = 0;
    contadorDer = 0;
  sei();
}



//    if (interruptPinDer) {
//      contadorDer = contadorDer + 1;
//    }
//
//    if (interruptPinIzq) {
//      contadorIzq = contadorIzq + 1;
//    }


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
