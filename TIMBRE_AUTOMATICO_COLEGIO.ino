//LIBRERIAS TIEMPO REAL ////
#include <Wire.h> 
#include "RTClib.h"
RTC_DS1307 RTC;

int pinBoton=2; //Pin al que se debe conectar el boton
int pinRelay=13; //Pin al que se debe conectar el relay
int TiempoSonado = 4000; //Tiempo que va a sonar el timbre (En milisegundos). 1000 ms = 1 seg
int aux=0;   //Variable donde se almacena el valor de la hora leida
int aux2=0; //Variable donde se almacena el valor del minuto leido
int aux3=0; //Variable para verificar que el timbre este sonando en esta hora
int aux4=0; //Variable para verificar que el timbre este sonando en este minuto
volatile byte diaSemana=false; //Variable para identificar si es un dia de semana
int dia=2; //Numero del dia: 1=lunes, 2=martes, 3=miercoles, 4=jueves, 5=viernes, 6=sabado, 7=domigno
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void setup()
{ 
  pinMode(pinBoton, INPUT_PULLUP);//Aqui se define el boton del pin como una entrada PULLUP
  pinMode(pinRelay, OUTPUT); //Aqui se define el Pin del relay como salida
  attachInterrupt(digitalPinToInterrupt(pinBoton), blink, RISING);//Se define el boton como una interrupcion
  Wire.begin(); //Se inicia la funcion Wire
  RTC.begin(); //Aqui se inicia la libreria del Reloj de tiempo real
  RTC.adjust(DateTime(__DATE__, __TIME__)); //Se ajusta el reloj al formato Fecha y hora
  AjusteDia();//Se llama la funcion que calcula el dia de la semana
  digitalWrite(pinRelay,LOW);//Se apaga el relay, para asegurarnos que no suena en primer momento
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void loop()
{
  DateTime now = RTC.now(); //Se hace lectura del reloj

  if(now.hour()==23 && now.minute()==59) {dia=dia+1; AjusteDia();} //Siempre que sea media noche se cambia el dia al siguiente
  
  while(diaSemana==true)//Si el dia, es un dia de semana entra
  {
    DateTime now = RTC.now();//Se hace nueva lectura del reloj
    while (now.hour()>=12 && now.hour()<19) //Mientras se encuentre en este horario el timbre funciona
    {
      horario();//Se llama la funcion donde estan las horas para sonar el timbre
      break;
    }
  }
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void horario() //Se compara la hora y minuto, si es verdadero va hacia la funcion de tiembre
{
  DateTime now = RTC.now(); //Se consulta el reloj nuevamente
  if(now.hour()==12 && now.minute()==45){ timbre(); } 
  if(now.hour()==13 && now.minute()==40){ timbre(); }
  if(now.hour()==14 && now.minute()==35){ timbre(); }
  if(now.hour()==15 && now.minute()==30){ timbre(); }
  if(now.hour()==16 && now.minute()==0 ){ timbre(); }
  if(now.hour()==16 && now.minute()==55){ timbre(); }
  if(now.hour()==17 && now.minute()==50){ timbre(); }
  if(now.hour()==18 && now.minute()==40){ timbre(); }  
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void blink()
{
  dia=dia+1;
  AjusteDia();
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void AjusteDia()
{
  if(dia>=1 && dia<=5) { diaSemana=true; }
  else { diaSemana=false;}
  if (dia==7) {dia=0;}
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void timbre() //Esta funcion hace que suene el timbre una sola vez
{
  DateTime now = RTC.now(); //Se consulta nuevamente la hora
  aux3=now.hour();//se almacena en aux3 la hora en que esta sonando el timbre
  aux4=now.minute(); //se almacena en aux4 el minuto en que esta sonando el timbre
  
  if(aux!=aux3 && aux2!=aux4) //compara si la hora en que esta sonando es diferente en la que ya sono anteriormente
  {
    aux=aux3;//Iguala la hora en que esta sonando con la que antes habia sonado
    aux2=aux4; //Iguala el minuto que esta sonando con el que antes habia sonado
    digitalWrite(pinRelay,HIGH);//Enciende el rele
    delay(TiempoSonado);//Mantiene encendido el rele
    digitalWrite(pinRelay,LOW); //Apaga el rele
  }
  else if(aux==aux3 && aux2!=aux4) //compara si la hora en que esta sonando es diferente en la que ya sono anteriormente
  {
    aux=aux3;//Iguala la hora en que esta sonando con la que antes habia sonado
    aux2=aux4;//Iguala el minuto que esta sonando con el que antes habia sonado
    digitalWrite(pinRelay,HIGH);//Enciende el rele
    delay(TiempoSonado);//Mantiene encendido el rele
    digitalWrite(pinRelay,LOW); //Apaga el rele
   }
}
