#include <Arduino.h>
//Definir las salidas que regulan el sentido del giro
//se definen salidas digitales
int salida1 = 22;
int salida2 = 23;
//Definir las salidas para el encendido del transformador
int salidaTrafo1 = 2;
int salidaTrafo2 = 3;
//Definir la salidas para encender la bombilla
int salidaBombillo = 4;
//Definir la salida que va a controlar la velocidad de giro
//Esta debe ser una señal PWM
int velocidadGiro = 5;
//Definir pin para leer la entrada del potenciómetro
int entradaPotenciometro = 6;

//Variablea para controlar los sobrepasos de los límites
bool limite1 = false;
bool limite2 = false;
bool limite3 = false;
void setup() {
  //Se declaran los pines como salidas
  pinMode(22,OUTPUT);
  pinMode(23,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  //Se declara la entrada para el potenciómetro
  pinMode(5,INPUT);
  Serial.begin(9600);
}

void loop() {
  int valorResistenciaPotenciometro = analogRead(6);
  
  int salidaControlVelocidadMotor = map(valorResistenciaPotenciometro,0,1023,0,255);
  //El primer punto de control será de 33% o 84 en medida análoga y se habilitará el transformador
  if(salidaControlVelocidadMotor >= 84 && salidaControlVelocidadMotor <= 166){
    //Se activa el transformador
    digitalWrite(salidaTrafo1,HIGH);
    digitalWrite(salidaTrafo2,HIGH);
    limite1 = true;
    //El segundo punto de control será de 66% o 166 en medida análoga y se habilitará la bombilla
  }
  if(salidaControlVelocidadMotor > 166 && salidaControlVelocidadMotor < 230){
    //Se activa la bombilla
    digitalWrite(salidaBombillo,HIGH);
    limite2 = true;
  }
  //Girar motor en sentido horario se deben declarar la salida 22 como HIGH y 23 como LOW
  if(salidaControlVelocidadMotor > 0 && salidaControlVelocidadMotor < 230 && limite1 == false && limite2 ==false){
    digitalWrite(salida1,HIGH);
    digitalWrite(salida2,LOW);
    limite3 = true;
  }else if(salidaControlVelocidadMotor > 230 && limite1 == true && limite2 == true){
    //Paramos el motor
    digitalWrite(salida1,LOW);
    digitalWrite(salida2,LOW);
    delay(1000);
    //Cambiamos el sentido del giro declarando 22 como LOW y 23 como HIGH
    digitalWrite(salida1,LOW);
    digitalWrite(salida2,HIGH);
  }else if(salidaControlVelocidadMotor <= 230 && limite3 == true){
    //Paramos el motor
    digitalWrite(salida1,LOW);
    digitalWrite(salida2,LOW);
    //volvemos a hacer girar en el mismo sentido que al comienzo
    digitalWrite(salida1,HIGH);
    digitalWrite(salida2,LOW);
    limite3 = false;
  }else if(salidaControlVelocidadMotor <= 166 && limite2 == true){
    digitalWrite(salidaBombillo,LOW);
    limite2 = false;
  }else if(salidaControlVelocidadMotor <= 84 && limite1 == true){
    digitalWrite(salidaTrafo1,LOW);
    digitalWrite(salidaTrafo2,LOW);
  }
}