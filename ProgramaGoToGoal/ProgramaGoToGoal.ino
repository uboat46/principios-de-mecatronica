#include <math.h>                                        // necesaria para utilizar función atan()
#define PI 3.1415926535897932384626433832795             // definición del número PI

int N = 20;                                              // nùmero de ranuras del encoder
int contadorTicks = 1;                                  // número de ticks para cálculo de velocidad (recordar que entre menor sea el valor mayor ruido de la medida)
int tam = 10;                                           // tamaño del vector del calculo de promedio (Este valor depende del tamaño de los vectores de promedio vectorL y vectorR)
int k = 10;                                             // tiempo de muestreo

volatile unsigned muestreoActual = 0;                     // variables para definiciòn del tiempo de muestreo
volatile unsigned muestreoAnterior = 0;
volatile unsigned deltaMuestreo = 0;

float error = 0;                                        // error variables
float Kp = 40;                                          // Contante proporcional control
int PWMr = 0;                                           // PWM de la llanta derecha (señal de control llanta derecha)
int PWMl = 0;                                           // PWM de la llanta izquierda (señal de control llanta izquierda)

int PWMmax=200;                                          // PWM màximo 
int PWMmin=20;                                           // PWM mìnimo


///------------------------------- Variables Posición del robot---------------------------------------------
float Cdistancia = 0;                                   // distancia recorrido punto central
float x = 0;                                            // distancia recorrida eje X
float y = 0;                                            // distancia recorrida eje Y
float phi = 0;                                          // posición angular

///------------------------------- Variables Posición deseada ---------------------------------------------
float Xd = 100;
float Yd = 100;
float Phid= atan2(Yd-y, Xd-x);

///------------------------------- Variables del robot  ---------------------------------------------

float diametro = 6.8;                                    // diametro de la llanta cm
float longitud = 13.4;                                   // longitud del robot entre llantas
float V = 0;                                             // Velocidad lineal del carro
float W = 0;                                             // Velocidad Angular del carro

///------------------------------- Variables de motor derecho---------------------------------------------

volatile unsigned muestreoActualInterrupcionR = 0;        // variables para definiciòn del tiempo de interrupciòn y calculo de la velocidad motor derecho
volatile unsigned muestreoAnteriorInterrupcionR = 0;
double deltaMuestreoInterrupcionR = 0;

int encoderR = 3;   // pin de conexiòn del encoder derecho
int llantaR = 9;      // pin de conexiòn de llanta derecha   (pin de PWM)

double frecuenciaR = 0;                                  // frecuencia de interrupciòn llanta R
double Wr = 0;                                           // Velocidad angular R
double Vr = 0;                                           // velocidad Lineal
int CR = 0;                                             // contador ticks
float vectorR[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    // vector de almacenamiento de datos para promedio del tiempo de interrupciones

float Rdistancia = 0;                                    // distancia recorrida llanta derecha
int Rtick = 0;                                           // ticks del encoder derecho
int RtickAnt = 0;                                        // ticks del encoder derecho anteriores
int deltaRtick = 0;                                      // diferencia del encoder derecho

//------------------------------  Variables de motor Izquierdo ------------------------------------------------

volatile unsigned muestreoActualInterrupcionL = 0;        // variables para definiciòn del tiempo de interrupciòn y calculo de la velocidad motor Izquierdo
volatile unsigned muestreoAnteriorInterrupcionL = 0;
double deltaMuestreoInterrupcionL = 0;

int encoderL = 2;   // pin de conexiòn del encoder Izquierdo
int llantaL = 8;      // pin de conexiòn de llanta Izquierda   (pin de PWM)

double frecuenciaL = 0;                                  // frecuencia de interrupciòn llanta Izquierda
double Wl = 0;                                           // Velocidad angular L
double Vl = 0;                                           // velocidad Lineal
int CL = 0;                                              // contador Ticks
float vectorL[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    // vector de almacenamiento de datos para promedio del tiempo de interrupciones

float Ldistancia = 0;                                    // distancia recorrida llanta izquierda
int Ltick = 0;                                           // ticks del encoder izquierdo
int LtickAnt = 0;                                        // ticks del encoder izquier anteriores
int deltaLtick = 0;                                      // diferencia del encoder izquierdo

void setup() {
  pinMode(encoderR, INPUT_PULLUP);
  pinMode(encoderL, INPUT_PULLUP);

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
  
  attachInterrupt(digitalPinToInterrupt(encoderR),REncoder,RISING);                // linea para añadir una interrupciòn a un PIN
  attachInterrupt(digitalPinToInterrupt(encoderL),LEncoder,RISING);                // linea para añadir una interrupciòn a un PIN
  Serial.begin(9600);                                                               // inicio de la comunicaciòn serial
}

void REncoder() {                                                                                         // función de interrupción del enconder llanta derecha
      Rtick++;                                                                                           // Nùmero de ticks llanta derecha
      CR++;                                                                                               // incremento del contador de ticks
      if (CR == contadorTicks){                                                                           // si el contador de ticks alcanza el valor de ticks determinado para el cálculo del tiempo
          float media = 0;                                                                                // variable creada para cálculo del promedio  
//-------------------------------------- -----------------------------    Filtro promedio    -----------------------------------------------------------------------------//
          for(int i=0;i < tam-1;i++){                                                                    // relleno del vector para cálculo posterior del promedio
              vectorR[i]=vectorR[i+1];                                                                   
          }
          vectorR[tam-1]=deltaMuestreoInterrupcionR ;                                                     // ùltimo dato del vector (medida actual) 

          for(int i=0;i<tam;i++){                                                                        // Suma de los valores del vector
              media = vectorR[i]+ media;
          }
          media = media/tam;                                                                             //división por el total de datos del vector
          deltaMuestreoInterrupcionR = media;                                                            // se reemplaza por el valor de su medío. 
//-------------------------------------- ----------------------------- ---------------------------------------------------------------------------------------------------//           
          frecuenciaR = (1000)/ deltaMuestreoInterrupcionR;                                              // frecuencia de interrupciòn      
          muestreoAnteriorInterrupcionR = muestreoActualInterrupcionR;                                   // se actualiza el tiempo de interrupciòn anterior
          CR = 0;                                                                                        //Reinicio de contador de ticks
      } 
 } 

void LEncoder() {                                                                                       // funciòn de interrupciòn del enconder llanta izquierda
      Ltick++;                                                                                           // Nùmero de ticks llanta izquierda
      CL++;                                                                                             // incremento del contador de ticks
      if (CL == contadorTicks){                                                                         // si el contador de ticks alcanza el valor de ticks determinado para el cálculo del tiempo
          float media = 0;                                                                              // variable creada para cálculo del promedio
//-------------------------------------- -----------------------------    Filtro promedio    -----------------------------------------------------------------------------//
          for(int i=0;i < tam-1;i++){                                                                    // relleno del vector para calculo posterior del promedio
              vectorL[i]=vectorL[i+1];
          }
          vectorL[tam-1]=deltaMuestreoInterrupcionL;                                                     // último dato del vector (medida actual) 

          for(int i=0;i<tam;i++){                                                                        // Suma de los valores del vector
              media = vectorL[i]+ media;
          }
          media = media/tam;                                                                             //división por el total de datos del vector
          deltaMuestreoInterrupcionL = media;                                                            // se reemplaza por el valor de su medío. 
//-------------------------------------- ----------------------------- ---------------------------------------------------------------------------------------------------//      
          frecuenciaL = (1000)/ deltaMuestreoInterrupcionL;                                              // frecuencia de interrupciòn 
          muestreoAnteriorInterrupcionL = muestreoActualInterrupcionL;                                   // se actualiza el tiempo de interrupciòn anterior
          CL = 0;                                                                                        // Reinicio de contador de ticks
       } 
 } 


void loop() { 
    muestreoActual = millis();                                                                           //Tiempo actual de muestreo
    muestreoActualInterrupcionR = millis();                                                              // se asigna el tiempo de ejecuciòn a el muestreo actual
    muestreoActualInterrupcionL = millis();                                                              // se asigna el tiempo de ejecuciòn a el muestreo actual

    deltaMuestreo =(double) muestreoActual - muestreoAnterior;                                           // delta de muestreo 
    if ( deltaMuestreo >= k)                                                                             // se asegura el tiempo de muestreo
    {   
        float Phid= atan2(Yd-y, Xd-x);                                                                   // Recalcular el ángulo deseado en cada iteración, dado que el cambia con respecto  a cada movimiento
         
        deltaMuestreoInterrupcionR = muestreoActualInterrupcionR -  muestreoAnteriorInterrupcionR;       // diferencia tiempos de interruciones de ticks del motor     
        deltaMuestreoInterrupcionL = muestreoActualInterrupcionL -  muestreoAnteriorInterrupcionL;       // diferencia tiempos de interruciones de ticks del motor     

        if(deltaMuestreoInterrupcionR >= 200*contadorTicks){                                              // Esta es la forma de definir cuando el motor se encuentra quieto. Si deltaMuestreoInterrupcionR es mayor a 40 milisegundos por el preescalado de ticks
          frecuenciaR=0;                                                                                  // 40 mS es el tiempo que màximo se tarda un tick a la menor velocidad del motor
        }
        if(deltaMuestreoInterrupcionL >= 200*contadorTicks){                                              // Esta es la forma de definir cuando el motor se encuentra quieto. Si deltaMuestreoInterrupcionR es mayor a 40 milisegundos por el preescalado de ticks
          frecuenciaL=0;                                                                                  // 40 mS es el tiempo que màximo se tarda un tick a la menor velocidad del motor
        }

        Wr = contadorTicks*((2*PI)/N)*frecuenciaR;                                                        // frecuencia angular Rad/s
        Vr= Wr*(diametro/2);                                                                              // velocidad lineal cm/s
        Wl = contadorTicks*((2*PI)/N)*frecuenciaL;                                                        // frecuencia angular Rad/s
        Vl= Wl*(diametro/2);                                                                              // velocidad lineal cm/s    

//        V = (Vr+Vl)/2;                                                                                    // calculo de la velocidad del robot
        V = 50;                                                                                           // velocidad constante para alcanzar el àngulo
        error = Phid - phi;                                                                               // error angular Angulo deseado menos el angulo del robot
        W = (Vr-Vl)/longitud + Kp * error;                                                                // Càlculo de la velocidad angular con las variables de control
        PWMr = V + (W*longitud)/2;                                                                        // Señal de control PWM llanta derecha
        PWMl = V - (W*longitud)/2;                                                                        // Señal de control PWM llanta izquierda

//-------------------------------------- condicionales para limites de la señal de PWM ---------------------------------------------------------------------------------------------------//  

        if(PWMr > PWMmax){                                                                               
           PWMr = PWMmax;
        }
        if(PWMr < PWMmin){
          PWMr = PWMmin;
        }
        if(PWMl > PWMmax){
           PWMl = PWMmax;
        }
        if(PWMl < PWMmin){
           PWMl = PWMmin;
        } 

        if( abs(x-Xd) < 5 && abs(y-Yd) < 5){
          analogWrite(llantaR,0);
          analogWrite(llantaL,0); 
        }
        else {
          analogWrite(llantaR,PWMr);
          analogWrite(llantaL,PWMl); 
       }
        
//        analogWrite(llantaR,PWMr);                                                                           // PWM de la llanta derecha
//        analogWrite(llantaL,PWMl);                                                                           // PWM de la llanta izquierda

//        analogWrite(llantaR,0);                                                                           // PWM de la llanta derecha
//        analogWrite(llantaL,0);                                                                           // PWM de la llanta izquierda
        
        odometria();                                                                                      // cálculo de la odometría                 
        
        Serial.print(x);                                                                                  // se muestra el tiempo entre TIC y TIC
        Serial.print(",");                                                                                 // se muestra el tiempo entre TIC y TIC
        Serial.println(y);                                                                                // se muestra el tiempo entre TIC y TIC
       
        muestreoAnterior = muestreoActual;                                                                 // actualización del muestreo anterior
     }
}

void odometria(){ 

   deltaRtick = Rtick - RtickAnt;                                                                         // comparación de los ticks recorridos desde el último cálculo llanta derecha               
   Rdistancia = PI*diametro*(deltaRtick/(double) 20);                                                     // distancia recorrida por la llanta derecha desde el último cálculo

   deltaLtick = Ltick - LtickAnt;                                                                         // comparación de los ticks recorridos desde el último cálculo llanta izquierda      
   Ldistancia = PI*diametro*(deltaLtick/(double) 20);                                                     // distancia recorrida por la llanta izquierda desde el último cálculo   

   Cdistancia = (Rdistancia + Ldistancia)/2;                                                               // distancia recorrida por el punto central desde el último cálculo  

   x = x + Cdistancia*cos(phi);                                                                            // posición del punto X actual
   y = y + Cdistancia*sin(phi);                                                                            // posición del punto Y actual
   
   phi = phi + ((Rdistancia - Ldistancia)/longitud);                                                       // posición Angular actual
   phi = atan2(sin(phi),cos(phi));                                                                         //transformación de la posición angular entre -PI y PI

   RtickAnt = Rtick;                                                                                       // actualización de la variable RtickAnt con los valores de Rtick
   LtickAnt = Ltick;                                                                                       // actualización de la variable LtickAnt con los valores de Ltick
 } 
