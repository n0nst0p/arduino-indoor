
/**
              Control Indoor para plantas.

 Este control cuenta con las siguientes prestaciones:

 - Control de la luz ON/OFF en distintos periodos para cada estapa de las plantas
      Para las autoflorecientes 20 horas de luz y 4 horas de oscuridad
      Para las feminizadas 18 horas de luz y 6 horas de oscuridad para la estapa de crecimiento y para la floracion 12 horas de luz y 12 de oscuridad.
 - Control de temperatura y humedad.          
      Control de la temperatura y humedad en el dia y la noche
 - Seleccion de que tipo de plantas se controlaran
      Para autoflorecientes el control es para 30 dias, cada 30 dias el mismo control se repite, 20/4.
      Para feminizadas el control es para 70 dias, los primeros 30 dias el fotoperiodo es de 18/6 estapa de crecimiento y los siguientes 40 dias es de 12/12 estapa de floracion.
 - Cuenta los dias transcurriodos dependiendo de cada estapa de la planta, ante una desconexion del arduino lee el ultimo dia de la etapa en la EEPROM del arduino uno.
 - Detecta si la planta necesita agua.
 - Seleccion de que tipo de plantas son, feminizadas o autoflorecientes en el pin digital 8. 
 
 ********************************** Es posible realizar el cambio de las horas de luz al compilar el codigo **********************************************
 
 ********************************************************Para probar el codigo****************************************************************************
 
 ** Para realizar pruebas con los horarios, dias, etapas, agregar el dia de avance en el codigo " void setup() EEPROM.write(address, 69); " 
 ** En este ejemplo esta en etapa de floracion, dia 69.  Luego cambiando la fecha en el reloj del PC en el cual compilan y luego cargando 
 ** el codigo para fijar la fecha en el Tiny RTC I2C DS1307. Asi es posible comprobra el correcto funcionamiento antes de la puesta en marcha.
 
*********************************************************************************************************************************************************** 
 
Detalles:

  - Sensor de temperatura y humedad SHT15
  - Reloj Tiny RTC I2C DS1307
  - Sensor humedad sel sustrato (Tierra)
  - Arduino uno R3
  - Bomba de agua. Cualquiera, se activa en el pin digital 6
  - Ventilacion y estraccion del aire se activan con el pin digital 7 y 3 respectivamente. (Con un puente H es posible utilizar el mismo Fan)
  - Calefaccion. cualquier calefactor, se activa con el pin digital 5.
  - Vaporizacion. puede ser cualquier vaporizador, este se activa con el pin digital 4.
  - Luz. esta es a su gusto, yo arme un panel con 576 led, 192 de cada color, rojo, azul y blaco de alto brillo. La luz se activa con el pin digital 2.

 
 andnitro@gmail
 
 */


#include <SHT1x.h>
#include <Wire.h> 
#include "RTClib.h"
#include <EEPROM.h>
#define address 0
#define dataPin  A2
#define clockPin A3
int sensorPin = A1;     // Sensor de agua configurado en el pin analogo A1 
int sensorValue = 0;    // Almacena el valor 0 en la variable sensorValue para el sensor de agua
int modo = 8;           // Para seleccionar el modo de cultivo. autoflorecientes o feminizadas. en el pin 8
int ventitacion = 7;    // Para activar la ventilacion en el pin digital 7
int bomba = 6;          // Para activar la bomba de agua en el pin digital 6
int calefaccion =  5;   // Para activar la calefaccion con el pin digita 5
int vaporizacion = 4;   // Para activar el evaporizador con el pin digital 4
int extraccion  =  3;   // Para activar la extraccion del aire con el pin digital 3
int luz  =  2;          // Para activar la extraccion del aire con el pin digital 2

int dias;               // Para contar los dias transcurridos
int conta;              //¨Para verificar el siguinte dia

SHT1x sht1x(dataPin, clockPin);
RTC_DS1307 RTC;

void setup()
{
    Serial.begin(9600); 
    Wire.begin();            // Inicia el puerto I2C
    RTC.begin();             // Inicia la comunicación con el RTC
    
    pinMode(ventitacion, OUTPUT);           // pone el pin como salida
    pinMode(bomba, OUTPUT);                 // pone el pin como salida
    pinMode(calefaccion, OUTPUT);           // pone el pin como salida
    pinMode(vaporizacion, OUTPUT);          // pone el pin como salida
    pinMode(luz, OUTPUT);                   // pone el pin como salida
    pinMode(modo, INPUT);                   // pone el pin como entrada
    
    
    //EEPROM.write(address, 69);              // Para realizar pruebas con los cambios de estapas. 
    
}
 
void loop()
{  
  DateTime now = RTC.now();
  conta = now.day();
  dias = EEPROM.read(address);

  if (digitalRead(modo) == HIGH)
  {
    control_mensual_crecimiento();
    control_mensual_floracion();     
  }
  else if (digitalRead(modo) == LOW)
  {
    control_mensual_autofloreciente();
  }
  
  //fecha();
  //tem_hum_dia();
  //tem_hum_noche();
  //agua();
  //cambio_aire();
  //periodo_luz_crecimiento();
  //periodo_luz_floracion();
  //periodo_luz_autofloreciente();
  
}

void control_mensual_autofloreciente()
{
    while (dias < 30)
    {
      DateTime now = RTC.now();
      
      Serial.println("Bucle autofloreciente");
      fecha();
      
      periodo_luz_autofloreciente();
      
      if (now.hour() >= 4 && now.hour() <= 23)
        {
          tem_hum_dia();  
        }
        else if (now.hour() >= 00 && now.hour() <= 3)
        {
          tem_hum_noche();
        }
      Serial.print("Valor Conta: ");
      Serial.println(conta);
      Serial.print("Valor Dias: ");
      Serial.println(dias);  
      
      cambio_aire();
      agua();
      
      if (now.day() - conta == 1)
      {
        dias++;
        EEPROM.write(address, dias);
        conta = now.day();        
      }   
    }
 dias = 0;
 EEPROM.write(address, dias);
}

void control_mensual_crecimiento()
{
    while (dias < 30)
    {
      DateTime now = RTC.now();
      
      Serial.println("Bucle crecimiento");
      fecha();
      
      periodo_luz_crecimiento();
      
      if (now.hour() >= 6 && now.hour() <= 23)
        {
          tem_hum_dia();  
        }
        else if (now.hour() >= 00 && now.hour() <= 5)
        {
          tem_hum_noche();
        }
      Serial.print("Valor Conta: ");
      Serial.println(conta);
      Serial.print("Valor Dias: ");
      Serial.println(dias); 
      
      cambio_aire();
      agua();
      
      if (now.day() - conta == 1)
      {
        dias++;
        EEPROM.write(address, dias);
        conta = now.day();        
      }   
    }
 EEPROM.write(address, dias);
}

void control_mensual_floracion()
{
    while (dias < 70)
    {
      DateTime now = RTC.now();
      
      Serial.println("Bucle floracion");
      fecha();
      
      periodo_luz_floracion();
      
      if (now.hour() >= 12 && now.hour() <= 23)
        {
          tem_hum_dia();  
        }
        else if (now.hour() >= 00 && now.hour() <= 11)
        {
          tem_hum_noche();
        }
      Serial.print("Valor Conta: ");
      Serial.println(conta);
      Serial.print("Valor Dias: ");
      Serial.println(dias); 
      
      cambio_aire();
      agua(); 
      
      if (now.day() - conta == 1)
      {
        dias++;
        EEPROM.write(address, dias);
        conta = now.day();        
      }   
    }
 dias = 0;
 EEPROM.write(address, dias);
}

void tem_hum_dia()
{
  float temperatura;
  float humedad;
  
  temperatura = sht1x.readTemperatureC();
  humedad = sht1x.readHumidity();
  
  Serial.print("Temperatura Humedad Dia: ");
  Serial.print(temperatura, DEC);
  Serial.print("  -  ");
  Serial.println(humedad, DEC);
  
if (temperatura >= 25)
    {
      digitalWrite(ventitacion, HIGH);
      delay(30000);
      digitalWrite(ventitacion, LOW);  
    } 
    else if (temperatura  <= 20)
    {
      digitalWrite(calefaccion, HIGH);
      delay(30000);
      digitalWrite(calefaccion, LOW);  
    }  
    
if (humedad >= 65)
    {
      digitalWrite(ventitacion, HIGH);
      delay(30000);
      digitalWrite(ventitacion, LOW);  
    } 
    else if (humedad  <= 45)
    {
      digitalWrite(vaporizacion, HIGH);
      delay(30000);
      digitalWrite(vaporizacion, LOW);  
    }
}

void tem_hum_noche()
{
  float temperatura;
  float humedad;
  
  temperatura = sht1x.readTemperatureC();
  humedad = sht1x.readHumidity();

  Serial.print("Temperatura Humedad Noche: ");
  Serial.print(temperatura, DEC);
  Serial.print("  -  ");
  Serial.println(humedad, DEC);
  
if (temperatura >= 20)
    {
      digitalWrite(ventitacion, HIGH);
      delay(60000);
      digitalWrite(ventitacion, LOW);  
    } 
    else if (temperatura  <= 15)
    {
      digitalWrite(calefaccion, HIGH);
      delay(60000);
      digitalWrite(calefaccion, LOW);  
    }  
if (humedad >= 65)
    {
      digitalWrite(ventitacion, HIGH);
      delay(30000);
      digitalWrite(ventitacion, LOW);  
    } 
    else if (humedad  <= 45)
    {
      digitalWrite(vaporizacion, HIGH);
      delay(30000);
      digitalWrite(vaporizacion, LOW);  
    }
}

void agua()
{
  sensorValue = analogRead(sensorPin);
  
  Serial.print("Agua: ");                  
  Serial.println(sensorValue);  
  
if (sensorValue <= 300 && sensorValue >= 0)
    {
        digitalWrite(bomba, HIGH);
        delay(30000);
        digitalWrite(bomba, LOW);   
    }
}

void cambio_aire()
{
        Serial.println("Cambiando el Aire ");
        digitalWrite(extraccion, HIGH);
        delay(60000);
        digitalWrite(extraccion, LOW);
        delay(5000);
        digitalWrite(ventitacion, HIGH);
        delay(60000);
        digitalWrite(ventitacion, LOW);
}
void periodo_luz_crecimiento()
{
  DateTime now = RTC.now(); // Obtiene la fecha y hora del RTC
  
  Serial.println("Periodo de Luz Crecimiento, Verificando Estado.");
  
  if (now.hour() >= 6 && now.hour() <= 23)
  {
    digitalWrite(luz, HIGH);  
  }
  else if (now.hour() >= 00 && now.hour() <= 5)
  {
    digitalWrite(luz, LOW);
  }
} 
void periodo_luz_floracion()
{
  DateTime now = RTC.now(); // Obtiene la fecha y hora del RTC
  
  Serial.println("Periodo de Luz Floracion, Verificando Estado.");
  
  if (now.hour() >= 12 && now.hour() <= 23)
  {
    digitalWrite(luz, HIGH);  
  }
  else if (now.hour() >= 00 && now.hour() <= 11)
  {
    digitalWrite(luz, LOW);
  }
} 
void periodo_luz_autofloreciente()
{
  DateTime now = RTC.now(); // Obtiene la fecha y hora del RTC
  
  Serial.println("Periodo de Luz Autoflorecientes, Verificando Estado.");
  
  if (now.hour() >= 4 && now.hour() <= 23)
  {
    digitalWrite(luz, HIGH);  
  }
  else if (now.hour() >= 00 && now.hour() <= 3)
  {
    digitalWrite(luz, LOW);
  }
} 

void fecha()
{
    DateTime now = RTC.now(); // Obtiene la fecha y hora del RTC
    
    if (now.day() <= 9)
    {
      Serial.print("0");
    }
    Serial.print(now.day(), DEC); // Dia
    Serial.print('/');
    if (now.month() <= 9)
    {
      Serial.print("0");
    }
    Serial.print(now.month(), DEC); // Mes
    Serial.print('/');
    Serial.print(now.year(), DEC); // Año
    Serial.print(' ');
    if (now.hour() <= 9)
    {
      Serial.print("0");
    }
    Serial.print(now.hour(), DEC); // Horas
    Serial.print(':');
    if (now.minute() <= 9)
    {
      Serial.print("0");
    }
    Serial.print(now.minute(), DEC); // Minutos
    Serial.print(':');
    if (now.second() <= 9)
    {
      Serial.print("0");
    }
    Serial.print(now.second(), DEC); // Segundos
    Serial.println("  ");
}


