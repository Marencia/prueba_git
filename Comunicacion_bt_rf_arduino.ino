#include <RH_ASK.h>    // incluye libreria RadioHead.h
#include <SPI.h>    // incluye libreria SPI necesaria por RadioHead.h

int sensor_IN = 9;//Sensor de entrada
int sensor_OUT = 11;//sensor de salida
int led=13;//Led de prueba para comprobar los datos en el emisor
char val;//variable para bluetooth
boolean gancho;// variable para while
//pin 12 para data de emisor

RH_ASK rf_driver;   // crea objeto para modulacion por ASK

void setup() //Configuracion de entradas, salidas y velocidad de comunicación
{
  rf_driver.init();   // inicializa objeto con valores por defecto
  pinMode(sensor_IN, INPUT);
  pinMode(sensor_OUT, INPUT);
  pinMode (led, OUTPUT);
  delay(2000);//delay para comenzar el programa
  Serial.begin (9600);
  }

void loop() {
  //BLUETOOTH
  if( Serial.available() ) {//Pregunta si hay algún dispositivo conectado al HC-05
  val = Serial.read();//Lee los datos enviados a través del celular
    switch (val){
      case '0': 
              Serial.print("SALIENDO ");
              Serial.println(val);
      break;
    
      case '3':
              Serial.print("ENTRANDO ");
              Serial.println(val);
              gancho=true; 
              while (gancho==true){//ciclo para mantener la opción de ON activa
               
               //EMISOR_RF Y SENSORES
                  if (digitalRead(sensor_IN) == HIGH){
                  const char *msg = "1";  // puntero de mensaje a emitir
                  rf_driver.send((uint8_t *)msg, strlen(msg));// funcion para envio del mensaje
                  rf_driver.waitPacketSent();     // espera al envio correcto
                  Serial.println("Movimiento detectado ");
                  delay(7000);}
            
                if (digitalRead(sensor_OUT) == HIGH){
                   const char *msg = "2";  // puntero de mensaje a emitir
                   rf_driver.send((uint8_t *)msg, strlen(msg));// funcion para envio del mensaje
                   rf_driver.waitPacketSent();     // espera al envio correcto
                   Serial.println("Movimiento de de salida detectado "); 
                   delay(7000);
                }
                val = Serial.read();
                if(val=='0'){//Para salir del while solo consulta si desde la aplicación ya se envió la instruccion de OFF
                   gancho=false;
                   Serial.print("SALIENDO ");
                   Serial.println(val);
                }
             }
      break;
          }
       }
    }

 


/*
Bueno acá comento como tendria que funcionar. El programa se separa en tres partes.
PRIMERA:
  Comunicación bluetooth: Envía a através de una aplicación del celular hecha con AppInventor,
  un mensaje de ENTRANDO A LA HABITACIÓN (val='3') o SALIENDO DE LA HABITACIÓN (val='0').
  Lo que debe hacer es que, cuando envío un '3', se entre al if val=='3' para ejecutar 
  todas las instrucciones que hacen funcionar a los sensores y a la comunicacion de 
  Radiofrecuencia. Si se envía el '0' debe salir del if val=='3'  y esperar un nuevo mensaje de '3' o nada.
  
SEGUNDA:
  Hay dos sensores, uno sensa la entrada de un pasillo (sensor_IN) == HIGH) y el otro sensa 
  la salida del mismo (sensor_OUT) == HIGH). En ambos casos se imprime en el monitor Serie
  el mensaje que pertenece a cada acción. 
  
TERCERA:
  Dentro de los if de los sensores, se configura la comunicación entre el emisor y receptor
  de radiofrecuencia. Esto hace que se envie un mensaje de '1' si se está entrando o un '2'
  si se está saliendo del pasillo.
  
Este es el funcionamiento general. El problema con este código es que cuando envio desde
la app el '3' ingresa al if pero solamente habilita los sensores una sola vez. Después
debe recibir si o si otro mensaje de '3' para verificar los if de sensor. O directamente
se cuelga y no hace más nada.
Con el while y la variable gancho que están comentados logro que los sensores esten siempre habilitados, osea 
que se quede dentro del if de val='3', pero solo sale de el cuando sensor_OUT está
en HIGH, y no cuando desde la app le envío '0'. 
La otra opción que pensaba era utilizar un case, pero es casi lo mismo que los if.
*/
