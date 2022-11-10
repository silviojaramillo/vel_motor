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

//Creando un arreglo para realizar la prueba
int myArray[] = {100,358,716,1020,716,358,100, 50};
int contador = 0;
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

  //Para realizar las pruebas vamos a alterar este valor mandando valores entre 0 y 1023

  //int valorResistenciaPotenciometro = analogRead(6);
  int valorResistenciaPotenciometro = myArray[contador];
  
  int salidaControlVelocidadMotor = map(valorResistenciaPotenciometro,0,1023,0,255);

  //Vamos a arrancar el motor desde cero hasta 83
  //Girar motor en sentido horario se deben declarar la salida 22 como HIGH y 23 como LOW
  if(salidaControlVelocidadMotor < 84 && limite1 == false && limite2 == false){
    //Ponemos en marcha el motor
    digitalWrite(salida1,HIGH);
    digitalWrite(salida2,LOW);
    Serial.println("El motor se ha puesto en marcha con valor de 100");
  }

  //El primer punto de control será de 33% o 84 en medida análoga y se habilitará el transformador
  if(salidaControlVelocidadMotor >= 84 && salidaControlVelocidadMotor <= 166 && limite1 == false){
    //Se activa el transformador
    digitalWrite(salidaTrafo1,HIGH);
    digitalWrite(salidaTrafo2,HIGH);
    limite1 = true;
    Serial.println("Se está ejecutando la segunda parte con el valor de 358 en escala 0-1023");
    //El segundo punto de control será de 66% o 166 en medida análoga y se habilitará la bombilla
  }
  if(salidaControlVelocidadMotor > 166 && salidaControlVelocidadMotor < 230 && limite1 == true && limite2 == false){
    //Se activa la bombilla
    digitalWrite(salidaBombillo,HIGH);
    limite2 = true;
    Serial.println("Se está ejecutando la tercera parte con el valor de 716 en escala 0-1023");
  }
  
  if(salidaControlVelocidadMotor > 230 && limite1 == true && limite2 == true){
    //Paramos el motor
    digitalWrite(salida1,LOW);
    digitalWrite(salida2,LOW);
    delay(1000);
    //Cambiamos el sentido del giro declarando 22 como LOW y 23 como HIGH
    digitalWrite(salida1,LOW);
    digitalWrite(salida2,HIGH);
    limite3 = true;
    Serial.println("Se está ejecutando la cuarta parte con un valor de 1020 en escala 0-1023");
  }
  if(salidaControlVelocidadMotor <= 230 && limite3 == true){
    //Paramos el motor
    digitalWrite(salida1,LOW);
    digitalWrite(salida2,LOW);
    //volvemos a hacer girar en el mismo sentido que al comienzo
    digitalWrite(salida1,HIGH);
    digitalWrite(salida2,LOW);
    limite3 = false;
    Serial.println("Se ha vuelto a velocidad por debajo del tercer punto de control");
  }

  if(salidaControlVelocidadMotor <= 166 && limite2 == true){
    digitalWrite(salidaBombillo,LOW);
    limite2 = false;
    Serial.println("Se ha vuelto a velocidad por debajo del segundo punto de control");
  }

  if(salidaControlVelocidadMotor <= 84 && limite1 == true){
    digitalWrite(salidaTrafo1,LOW);
    digitalWrite(salidaTrafo2,LOW);
    limite1 = false;
    Serial.println("Se ha vuelto a velocidad por debajo del primer punto de control");
  }
  Serial.println("Este es el valor del contador es");
  Serial.println(contador);
  contador++;
  delay(3000);
  if(contador >7){
    Serial.println("El programa se ha detenido");
    delay(2000);
    exit(0);
  }
}