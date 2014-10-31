arduino-indoor
==============


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
 
                            Es posible realizar el cambio de las horas de luz al compilar el codigo 
 
                            ************ Para probar el codigo **************
 
 ** Para realizar pruebas con los horarios, dias, etapas, agregar el dia de avance en el codigo " void setup()       EEPROM.write(address, 69); " 
 ** En este ejemplo esta en etapa de floracion, dia 69.  Luego cambiando la fecha en el reloj del PC en el cual compilan y luego cargando 
 ** el codigo para fijar la fecha en el Tiny RTC I2C DS1307. Asi es posible comprobra el correcto funcionamiento antes de la puesta en marcha.
 
*****************************************************************************************************************************
 
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
