#include <RH_ASK.h>    // incluye libreria RadioHead.h
#include <SPI.h>    // incluye libreria SPI necesaria por RadioHead.h
//SoftwareSerial 
int sensor_IN = 9;
int sensor_OUT = 11;
int led=13;

boolean gancho;
//pin 12 para data de emisor

RH_ASK rf_driver;   // crea objeto para modulacion por ASK

void setup() 
{
  rf_driver.init();   // inicializa objeto con valores por defecto
  pinMode(sensor_IN, INPUT);
  pinMode(sensor_OUT, INPUT);
  pinMode (led, OUTPUT);
  delay(5000);
  Serial.begin (9600);
  //digitalWrite(led, LOW);
  }

void loop() {
  static char val;
  //BLUETOOTH
  if( Serial.available() ) {
  val = Serial.read();
  
  if( val == '0' )
  { 
   Serial.print("SALIENDO ");
   Serial.println(val);
  }
  
  if( val == '3' )
  { 
   Serial.print("ENTRANDO ");
   Serial.println(val);
//   gancho=true; 
//   while (gancho==true){
     //EMISOR_RF Y SENSORES
  
      if (digitalRead(sensor_IN) == HIGH){
        const char *msg = "1";  // puntero de mensaje a emitir
        rf_driver.send((uint8_t *)msg, strlen(msg));// funcion para envio del mensaje
        rf_driver.waitPacketSent();     // espera al envio correcto
        delay(4000);        // demora de 1 segundo entre envios
        Serial.println("Movimiento detectado ");}
    
     if (digitalRead(sensor_OUT) == HIGH){
        const char *msg = "2";  // puntero de mensaje a emitir
        rf_driver.send((uint8_t *)msg, strlen(msg));// funcion para envio del mensaje
        rf_driver.waitPacketSent();     // espera al envio correcto
        delay(4000);        // demora de 1 segundo entre envios
        Serial.println("Movimiento de de salida detectado "); 
//        gancho=false;
     }
    }
  } 
 } 
