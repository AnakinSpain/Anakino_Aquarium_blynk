
///////////////////////////////////////////////////////////////
//
//
//
//
//     CONTROLADOR DE ACUARIO CON LA APP BLYNK 
//     V1.0 
//     
//
//
//
///////////////////////////////////////////////////////////////
//Listado de PINs utilizados
///////////////////////////////////////////////////////////////
///////// PINES INPUT///////////
//        Sondas temp.     23
//        Boya nivel agua  24
//        Boya nivel dep   25

/////// PINES  OUTPUT/////
//                             
//        ventilador       41
//        Bomba  relleno   42
//        Calentador       43
//        Aireador         44
//        Lampara UV       45
//        Luces            46

/////// PUERTOS VIRTUALES BLYNK /////
//                            
//        V1       Modo Airador
//        V2       Modo lamp UV
//        V3       Aviso rellenador funcionando
//        V4       calentador en on u off
//        V5       temp de agua
//        V6       temp habitacion
//        V7       temp pantalla
//        V8       Asigna valor a la temp. del acuario desde blynk
//        V9       Rellenador ON / OFF
//        V10      Disparador del temporizador del aireador
//        V11      Disparador del temporizador de la Lamp UV
//        
//       
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
//Librerias necesarias
///////////////////////////////////////////////////////////////
#include <Ethernet.h>
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BlynkSimpleEthernet.h>
#define BLYNK_PRINT Serial


////////////////////////////////////////////////////////////////////////
//CONFIGURACION ETHERNET
////////////////////////////////////////////////////////////////////////
/*
//IP manual settings
byte ip[] = {  192, 168, 1, 177 };   //Manual setup only
byte gateway[] = {  192, 168, 1, 1 }; //Manual setup only
byte subnet[] = {  255, 255, 255, 0 }; //Manual setup only
*/
// if need to change the MAC address (Very Rare)
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;

////////////////////////////////////////////////////////////////////////
//DECLARACION DE VARIABLES Y PINES
////////////////////////////////////////////////////////////////////////

int alarmas = 0; // alarmas del acuario

////////////////////////////////////////////////////////////////
// Definimos las salidas de los Reles a 220v
////////////////////////////////////////////////////////////////
#define calentador  43           // Calentador             
#define aireador  44           // Aireador    
#define lamp_uv  45           // Lampara UV
#define luz  46           // Luz

#define ventilador 41        // Ventilador pin 41

byte estado1;   // estado calentador   // valor del estado 0 = auto encendido
byte estado2;   // Estado aireador     // valor del estado 1 = auto apagado
byte estado3;   // Estado Lamp UV      // valor del estado 2 = man encendido
byte estado4;   // Estado rele libre   // valor del estado 3 = man apagado
byte estado5;   // Estado rellenador

////////////////////////////////////////////////////////////////
//*********************** Variables de control de temperatura del agua ********************
// Preparamos otras variable
float temp_agua;    //Temperatura del agua
float temp_agua_des; //temperatura agua deseada
int contador_temp = 0;
float temperatura_agua_temp;       // Temperatura temporal del agua
int temperatura_margen = 2;    //Margen de actuacion del calentador

 
//float tempHB;    //Temperatura de la habitacion
float temp2_temp; //temperatura temporal sistema

float temperatura_disipador_temp; // Temperatura temporal del disipador
float temperatura_habitacion_temp; // temperatura temporal de la habitacion
    
 float tempD = 0;     // Temperatura del disipador
 float tempHB = 0;    // Temperatura habitacion
 
//////////////////////////////////////////////////////////////
//************ Variables rellenador


 int nivel_estado = 1;
 const int nivel_acu = 24;  // Boya nivel agua acuario
 const int nivel_dep = 25;  // Boya nivel agua deposito
 const int bomba = 42;      // Pin accionara la bomba de llenado 

/*
float PinSensor = 30; //Sensor de nivel de agua en el pin 30
int VarSensor = 0;  //Control del nivel de agua
int floatPin = 31;  //Boya de nivel del depósito de relleno
*/
/////////////////////////////////////////////////
// Preparamos  modos de funcionamiento

int modo_relleno; // Rellenador 1 -> activado // 0-> desactivado
int modo_uv;     // modo de funcionamiento uv
int modo_ai;     // modo de funcionamiento aireador

int temp_uv;          // Variable para indicar si activa o no la lamp uv desde blynk por el temporizador
int temp_ai;          // Variable para indicar si activa o no el aireador desde blynk por el temporizador

int VentMax = 34;     // Temperatura de encendido Ventilador
int VentMin = 30;     // Temperatura de apagado Ventilador
int buzzer = 43;      // Pin de conexión del buzzer

/////////////////////////////////////////////////////////////////
//Blynk
////////////////////////////////////////////////////////////////
char auth[] = "4c9d2ca518b04366b3de5496f99c75dd";

  BLYNK_CONNECTED()     // This function will run every time Blynk connection is established
  {  
   Blynk.syncAll();  // Request Blynk server to re-send latest values for all pins
   }

    
  BLYNK_WRITE(V1) // Variable del modo AIREADOR
{
  int modo_ai_blynk = param.asInt(); 
  Serial.print("V1 modo_ai value is: ");
  Serial.println(modo_ai_blynk);
  modo_ai=modo_ai_blynk;
}

  BLYNK_WRITE(V2) //  Variable del modo LAMPARA UV
{
  int modo_uv_blynk = param.asInt(); 
  Serial.print("V2 modo_ai value is: ");
  Serial.println(modo_uv_blynk);
  modo_uv=modo_uv_blynk;
}

 WidgetLED led1(V3); //  estado del calentador
 WidgetLED led2(V4);  // estado de la bomba de relleno
 

  BLYNK_WRITE(V8)  // variable de la temperatura del agua deseada
{
  int temp_agua_des_blynk = param.asInt(); // Asigna un valor a la variable temp_agua_des desde el slider de la app
  Serial.print("V8 modo_ai value is:  ");
  Serial.println(temp_agua_des_blynk);
  temp_agua_des=temp_agua_des_blynk;
}

  BLYNK_WRITE(V9) // Variable del rellenador
{
  int modo_relleno_blynk = param.asInt(); 
  Serial.print("V9 modo_ai value is: ");
  Serial.println(modo_relleno_blynk);
  modo_relleno=modo_relleno_blynk;
}

  BLYNK_WRITE(V10) //Obtenemos la variable del aireador en HIGH si está dentro del temporizador
{
  int temp_ai_blynk = param.asInt(); 
  Serial.print("V10 modo_ai value is: ");
  Serial.println(temp_ai_blynk);
  temp_ai=temp_ai_blynk;
  
}

  BLYNK_WRITE(V11) //Obtenemos la variable de la lamp uv en HIGH si está dentro del temporizador
{
  int temp_uv_blynk = param.asInt(); 
  Serial.print("V11 modo_ai value is: ");
  Serial.println(temp_uv_blynk);
  temp_uv=temp_uv_blynk;
  
}
 

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//Establecemos los PIN de los sensores de temperatura
///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


OneWire OneWireBus(23);      //Sensores de temperatura conectados al pin 23.
DallasTemperature sensors(&OneWireBus);
DeviceAddress sensor_agua, sensor_habitacion, sensor_disipador;
//DeviceAddress sensor_agua= {0x28, 0x10, 0x32, 0x2B, 0x04, 0x00, 0x00, 0x38 }; // Es necesario cambiar este valor acorde con nuestro sensor.
//DeviceAddress sensor_disipador = {0x28, 0x99, 0x47, 0x2B, 0x04, 0x00, 0x00, 0xFB }; // Es necesario cambiar este valor acorde con nuestro sensor.
//DeviceAddress sensor_habitacion = {0x28, 0x6D, 0x70, 0x2B, 0x04, 0x00, 0x00, 0x80 }; // Es necesario cambiar este valor acorde con nuestro sensor.

/*
/////////////////////////////////////////////////////////////////
// Variables de la sonda de PH
/////////////////////////////////////////////////////////////////
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;    
static float pHValue;
*/

