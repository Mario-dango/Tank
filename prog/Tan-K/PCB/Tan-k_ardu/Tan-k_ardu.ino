/***************Titulo
 * Autor: Mario Stefano Papetti Funes 
 * Versión: 1.0
 * Fecha: 10/10/2021
 * ***************************
 * El programa recibe argumentos tales cómo:
 * [A; R; D; I; S] Correspondientes al movimiento, vienen en conjunto con la potencia.
 *    Ejemplo: "A:30" -> El anque avanzará adelante con velocidad 30% nominal.
 *             "S" -> El tanque se detendra.
 * [T] correspondiente a petición de lectura a los termistores (NTC1 y NTC2).
 *    Ejemplo: "T:1" -> devuelve resistencia y temperatura actual de la NTC número 1.
 * [H] Corresponde a la petición de información del firmware.
 */

/*******************Extras o fuentes usadas para la construcción del algoritmo
 * 
 * Fuente de lectura de NTC
 *  https://www.geekfactory.mx/tutoriales-arduino/termistor-ntc-con-arduino-como-sensor-de-temperatura/
 * 
 */
//Inclusión de librerias para el programa
#include <AccelStepper.h>

//Declaraciones referentes a la naturaleza y origen del firmware
const char autor[] = "Mario Stefano Papetti Funes";
const char vers[] = "v1.0";
const char fecha[] = "10/10/2021";

//Etiquetas definidas para los pines que usan los drivers DRV8825 para controlar los motores
#define pulsosDER 5 //Motor derecho visto desde el frente
#define pulsosIZQ 6 //Motor izquierdo visto desde el frente
#define dirDER 7    //dirección del motor derecho
#define dirIZQ 4    //dirección del motor izquierdo
#define EnDER A5    //Enable motor Derecho
#define EnIZQ A4    //Enable motor Izquierdo
#define DRV8825 1   //Tipo de interfas del motor

#define Fault 8 //Entrada por si hay algún problema con los DRV8825

//Etiquetas definidas para el uso general de pines
#define NTC1 A2  //Entrada para sensar temperaturas o alguna LDR
#define NTC2 A3  //Entrada para sensar temperaturas o alguna LDR
#define temp_r 10000  //Resistencia en serie con el termistor para divisor de tensión
#define LedALR A1 //Salida led indicadora de que hay un problema
#define LedOKA A0 //Salida led indicadora de que está todo OKAY
//#define buzzer 9 //Salida del Parlante indicador
//const int speakerPin = 9;

//Cosntantes de nivel, para velocidades de pulsos para los motores
const int velMax = 1000;
const int cteVel = 200;
// variable para almacenar la temperatura y resistencia
float temp1 = 0;    //Temperatura correspondiente al termistor en NTC1
uint32_t res1 = 0;  //Resistencia actual correspondiente al termistor en NTC1
float temp2 = 0;    //Temperatura correspondiente al termistor en NTC2
uint32_t res2 = 0;  //Resistencia actual correspondiente al termistor en NTC2

//Creo los objetos motor Izquierdo y motor Derecho para posterior uso
AccelStepper motorIZQ = AccelStepper(DRV8825, pulsosIZQ, dirIZQ); //Creamos la instancia motor Izquierdo
AccelStepper motorDER = AccelStepper(DRV8825, pulsosDER, dirDER); //Creamos la instancia motor Derecho

//Pre defino funciones que se utilizan para movimiento o alerta visual
void alert(int tiempo);
boolean serial_decode(char* clase, int valor);
boolean mover(char* clase, int potencia);
boolean leer_temp(int ntc);
   
void setup(){
  /*
  Audio.speakerPin = 9; //Selecciona la salida de audio: pin 9 (UNO y nano) pines 5,6,11 o 46 (Mega)
  Audio.quality(1); // Mejoramos la calidad de sonido (puede ser 0 o 1)
  Audio.setVolume(5); // Se selecciona el volumen: valor entre 0 y 7. Solo funciona bien si la calidad del audio está a 1. 
                      //En caso de que no se oiga nada o se  oiga mal, bajad el volumen o borrad la funcion  
  */
   //inicializamos el puerto serie
   Serial.begin(9600);
   while(!Serial){};
  // Defino respectivos pines cómo entradas y salidas
   pinMode(Fault, INPUT);
   pinMode(LedOKA, OUTPUT);
   pinMode(LedALR, OUTPUT);
   pinMode(EnDER, OUTPUT);
   pinMode(EnIZQ, OUTPUT);
   digitalWrite(LedOKA, false);
   digitalWrite(LedALR, false);

  //Inicializo los objetos motores y los detengo
   motorIZQ.setMaxSpeed(velMax); //Definimos la velocidad maxima del motor izq
   motorDER.setMaxSpeed(velMax); //Definimos la velocidad maxima del motor der
   digitalWrite(EnDER, true);
   digitalWrite(EnIZQ, true);
   motorIZQ.setSpeed(0);
   motorDER.setSpeed(0);
}

void loop(){
  //Variables para movimiento
  int poder = 0;
  char modo = ' ';
  // Verifico si se enceuntra disponible la comunicación serial
  if (Serial.available()){
    //Defino algunas variables utiles para le decodificación del mensaje
    char mensaje = Serial.read();
    const char s[2] = ":";
    char *token;
    int cont = 0;
    
    /* get the first token */
    token = strtok(mensaje, s);
    
    /* walk through other tokens */
    while( token != NULL ) {      
      if (cont == 1){
        poder = atoi(token);
        printf("%i\n", poder);
        break;
      }else{
        modo = token;      
      }
      cont ++;
      token = strtok(NULL, s);
    }
    //Una vez descompuesto el string con el modo y la potencia se procede a actuar 
    // bajo verificación en el control del if.
    if (serial_decode(modo, poder)){
      printf("ok");
      digitalWrite(LedOKA, true);
      digitalWrite(LedALR, false);
    }else{
      printf("Error!");
      alert(150);
    }
  }
}

//Función derivadora del codigo recibido 
boolean serial_decode(char* codigo, int valor){
  if (codigo == 'T'){
    if ((valor == 1) || (valor == 2)){
      leer_temp(valor);
    }else {
      Serial.print("Error de parametro en Temperatura.");
      return false;
    }
  }else if (codigo == 'H'){
    //Pasa la información cargada del firmware, lo que es autor, versión y fecha de la última modificación.
    Serial.print("V:");
    Serial.println(vers);
    Serial.print("A:");
    Serial.println(autor);
    Serial.print("F:");
    Serial.println(fecha);
    return true;
  }else{
    return mover(codigo, valor);
  }
}

//Función encargada de lectura de los termistores
boolean leer_temp(int ntc){
  if (ntc == 1){
    // calcular la resistencia electrica del termistor usando la lectura del ADC
    res1 = get_res(analogRead(NTC1));
    // luego calcular la temperatura segun dicha resistencia
    temp1 = get_temp(res1);
    Serial.print("R1:");
    Serial.println(res1);
    Serial.print("T1:");
    Serial.println(temp1, 2);
  }else if (ntc == 2){
    // calcular la resistencia electrica del termistor usando la lectura del ADC
    res2 = get_res(analogRead(NTC2));
    // luego calcular la temperatura segun dicha resistencia
    temp2 = get_temp(res2);    
    Serial.print("R2:");
    Serial.println(res2);
    Serial.print("T2:");
    Serial.println(temp2, 2);
  }else {
    Serial.println("Error");
    return false;
  }
}

//Función encargada del movimiento del tanque
boolean mover(char* clase, int potencia){
  //analizando el valor de la variable potencia para determinar que clase de valores aparecen
  if (potencia != 0){
    clase = 'S';
  }else if (potencia > velMax){
    potencia = velMax;
  }else if ((potencia < cteVel) && (potencia > 10)){
    potencia = cteVel;
  }

  if (clase == 'S'){
    //Detenerse
    digitalWrite(EnDER, true);
    digitalWrite(EnIZQ, true);      
    motorDER.setSpeed(0);        
    motorIZQ.setSpeed(0);
  }
  if (clase == 'A'){
    //Avanzar
    digitalWrite(EnDER, false);
    digitalWrite(EnIZQ, false);  
    motorIZQ.setSpeed(velMax);
    motorDER.setSpeed(velMax);   
  }else if (clase == 'R'){
    //Retroceder
    digitalWrite(EnDER, false);
    digitalWrite(EnIZQ, false);  
    motorIZQ.setSpeed(-velMax);
    motorDER.setSpeed(-velMax);   
  }else if (clase == 'I'){
    //Girar a la izquierda
    digitalWrite(EnDER, false);
    digitalWrite(EnIZQ, false);
    motorIZQ.setSpeed(velMax);
    motorDER.setSpeed(0);  
  }else if (clase == 'D'){
    //Girar a la derecha
    digitalWrite(EnDER, false);
    digitalWrite(EnIZQ, false);    
    motorIZQ.setSpeed(0);
    motorDER.setSpeed(velMax);   
  }else {
    //stop
    digitalWrite(EnDER, true);
    digitalWrite(EnIZQ, true);      
    motorDER.setSpeed(0);        
    motorIZQ.setSpeed(0);
    alert(150);
    return false;
  }  
  
   motorIZQ.runSpeed();
   motorDER.runSpeed();
   return true;
}

//Función para dar destello de alerta en el led ALERTA
void alert(int tiempo){
  
  digitalWrite(EnDER, true);
  digitalWrite(EnIZQ, true);
  for (int i = 0; i < 2; i++){
  
    delay(tiempo);
    digitalWrite(LedOKA, false);
    delay(tiempo);
    digitalWrite(LedOKA, true);
  }
  return;
}
 
//Funciones para calcular la resistencia de los termistores
int32_t get_res(uint16_t adcval)
{
  // calculamos la resistencia del NTC a partir del valor del ADC
  return (temp_r * ((1023.0 / adcval) - 1));
}

//Función para el calculo de la temperatura en funcion de la resistencia actual
float get_temp(int32_t resistance)
{
  // variable de almacenamiento temporal, evita realizar varias veces el calculo de log
  float temp; 
  // calculamos logaritmo natural, se almacena en variable para varios calculos
  temp = log(resistance); 
  // resolvemos la ecuacion de STEINHART-HART
  // http://en.wikipedia.org/wiki/Steinhart–Hart_equation
  temp = 1 / (0.001129148 + (0.000234125 * temp) + (0.0000000876741 * temp * temp * temp)); 
  // convertir el resultado de kelvin a centigrados y retornar
  return temp - 273.15;
}
