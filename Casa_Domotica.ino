// LÍBRERIAS
#include <SoftwareSerial.h> //Libreria Software Serial para Bluetooth (Se descarga), en esta caso usamos el HC-05 que sirve para usarse de Esclavo ó Maestro
#include <LiquidCrystal.h>
#include <Wire.h>  //Librería para permitir comunicarse con el I2C
#include <Servo.h>  // librería para poder controlar el servo
#include <DHT.h>   //Librería para usar sensores DHT (Se descarga)
#include <DHT_U.h> //Complemento de la librería DHT


//LCD PINES
LiquidCrystal lcd_1(2, 4, 5, 9, 12, 13);

//PINES ESPECIALES

#define DHTPIN 6 //Se define el pin del DHT
#define trig 7 // Emisor de pulso o señal
#define echo 8 // Receptor "del eco" del pulso o señal
#define DHTTYPE DHT11 //Se define el tipo de sensor que se usara para medir la temperatura y humedad, en este caso sera el DHT-11

//OBJETOS DE LAS LIBRERÍAS
DHT dht(DHTPIN, DHTTYPE); // Crea un objeto de la librería DHT, indicando así el sensor
SoftwareSerial BT(10,11); //Definir los pines RX y TX del Arduino conectados al Bluetooth
Servo garage;   // Crea un objeto servo llamado servoMotor
Servo ventana;  //Crea un objeto servo llamado ventana
/*Servo puerta; //Crea un objeto Servo llamado puerta*/

//OBJETOS
char Mensaje; //Variable que mandara los mensajes a los Switches y por voz

// Bocina
const int buzzer = 33; //Bocina conectada al Pin 33

//Sistema de IR
const int sensorPin = 49; //Sensor de IR conectado al Pin 12 
const int LEDIV = 31; //Led Interior Verde
const int LEDIR = 30; //Led Interior Rojo
int luz = 0; //Valor para leer el dato de la luz       
int valor_sensor = 0; //Este sera el valor que andara leyendo la fotoresistencia       
int valor_limite = 475;  //Este valor hara que el LED cambie de estado a una determinada luminosidad.
int valor_luz;

//Sistema de luces exteriores
const int LEDE1 = 32; //Led Exterior 1
const int LEDE2 = 22; //Led Exterior 2
const int LEDE3 = 23; //Led Exterior 3
const int LEDE4 = 24; //Led Exterior 4
const int pinFoto = A0;  //El LDR esta conectado en el pin A0
int Intencidad = 0;

//Sistema de luces interiores
const int LEDR1 = 36; //Led Room 1
const int LEDR2 = 37; //Led Room 2
const int LEDR3 = 38; //Led Room 3
const int LEDP1 = 39; //Led Pasillo 1
const int LEDP2 = 40; //Led Pasillo 2
const int LEDP3 = 41; //Led Pasillo 3
const int LEDBO = 42; //Led Baño
const int LEDSA = 43; //Led Sala
const int LEDCO = 44; //Led Comedor
const int LEDCA = 45; //Led Cocina
const int LEDGA = 46; //Led Garage

//Luces para el Garaje
const int LEDVG = 47; //Led Verde Garage
const int LEDRG = 48; //Led Rojo Garage
const int tiempo;
const int distancia;

//Variables para almacenar las lecturas del sensor DHT11
float celsius;
float fahrenheit;
float humedad;

void setup() {
  Serial.begin(9600); 
  BT.begin(9600); //Inicializamos el puerto serie BT (Para Modo AT 2)
  dht.begin(); //Comunicacion con el DHT11
  lcd_1.begin(16, 2);  
  Serial.println("Iniciando todos los sistemas... Listo"); //Mensaje para indicar que esta el sistema listo
  lcd_1.setCursor(0,0);
  lcd_1.print("BIENVENIDO(A)"); //Mensaje de Bienvenida
  lcd_1.setCursor(3,1);
  lcd_1.print("DOMOTIC_APP"); //Mensaje de Bienvenida
  delay(5000);
  
  garage.attach(3); //Se conecta la variable del Servo Garage al pin 3
 // puerta.attach(3); //Se conecta la variable del Servo Puerta al pin 3
  ventana.attach(50); //Se conecta la variable del Servo Ventana al pin 13
  
  //Se establecen los pines como entradas o salidas
  //Todos los LED's se establecen como Salida
  //pinMode(sensorPin, INPUT); //Se establece como Entrada
  pinMode(buzzer, OUTPUT);
  pinMode(LEDIV,OUTPUT);
  pinMode(LEDIR,OUTPUT);
  
  pinMode(LEDE1,OUTPUT);   
  pinMode(LEDE2,OUTPUT);
  pinMode(LEDE3,OUTPUT);
  pinMode(LEDE4,OUTPUT);
  //pinMode(LDR,INPUT); //Se establece el LDR como Entrada

  pinMode(LEDR1, OUTPUT); 
  pinMode(LEDR2, OUTPUT);
  pinMode(LEDR3, OUTPUT); 
  pinMode(LEDP1, OUTPUT);
  pinMode(LEDP2,OUTPUT); 
  pinMode(LEDP3,OUTPUT); 
  pinMode(LEDBO,OUTPUT); 
  pinMode(LEDSA,OUTPUT); 
  pinMode(LEDCO,OUTPUT); 
  pinMode(LEDCA,OUTPUT); 
  pinMode(LEDGA,OUTPUT); 

  pinMode(LEDVG,OUTPUT); 
  pinMode(LEDRG,OUTPUT); 

  pinMode(trig, OUTPUT); //Se configura el Emisor como Salida
  pinMode(echo, INPUT); //Se configura el Receptor como Entrada
}

void loop() 
{ 
  long duration, distance; //Establecemos duration y distance como variables numéricas extensas

  digitalWrite(trig, LOW); //Para tener un pulso limpio empezamos con 2 microsegundos en apagado
  delayMicroseconds(1);
  digitalWrite(trig, HIGH); //Mandamos un pulso de 5 microsegundos
  delayMicroseconds(2);
  digitalWrite(trig, LOW); //Apagamos
  duration = pulseIn(echo, HIGH); //Medimos el tiempo que la señal tarda en volver al sensor en microsegundos
  distance = (duration/2)*0.0343; //La distancia es el tiempo por la velocidad del sonido (343 m/s = 0.0343 cm/microseg)
    
  digitalWrite(LEDVG,LOW); //Led verde indica que la puerta esta abierta
  digitalWrite(LEDRG,HIGH); //Led Rojo indicara que la puerta esta cerrada
  digitalWrite(LEDGA,HIGH);

  if (distance <10)
  { //Si la distancia es menor a un metro y medio, se abrira la puerta del garage 
    Serial.println("Puerta de Garage abierta");
    digitalWrite(LEDVG,HIGH);
    digitalWrite(LEDRG,LOW); //
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("AUTO LLEGANDO");
    lcd_1.setCursor(0,1);
    lcd_1.print("ABRIENDO GARAJE");
    delay(1000);
    tone(buzzer,600);
    delay(1000);
    noTone(buzzer);
    garage.write(0);
    delay(4000);
    tone(buzzer,500);
    delay(2000);
    noTone(buzzer);
    garage.write(-90);
    delay(1000);
  }
  else 
  {
    Serial.println("Puerta de Garage cerrada");
    digitalWrite(LEDVG,LOW);
    digitalWrite(LEDRG,HIGH);
    digitalWrite(LEDGA,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("GARAJE");
    lcd_1.setCursor(0,1);
    lcd_1.print("CERRADO");
    delay(1000);
    garage.write(90);                          
  }
  
  //SISTEMA DE TEMPERATURA Y HUMEDAD
  celsius = dht.readTemperature();  //Lee y almacena la temperatura del sensor DHT11
  fahrenheit = dht.readTemperature(true);  //Lee y almacena la temperatura del sensor DHT11
  humedad = dht.readHumidity();         //Lee y almacena la humedad del sensor DHT11
  valor_sensor = analogRead(A0);  //Lee el valor del sensor LDR
  valor_luz = map(valor_sensor, 1024, 0, 0, 255);  //Mapeo de los valores de la fotocelda en un rango de 0 a 255 %
  
  //Envía los datos del DHT11 como la temperatura, humedad e intensidad a la aplicación móvil por medio del Bluetooth
  BT.print(celsius);    
  BT.print(" °C");
  BT.print("|");
  BT.print(fahrenheit);
  BT.print(" °F");
  BT.print("|");
  BT.print(humedad);
  BT.print(" %");
  BT.print("|");
  BT.print(valor_luz);
  BT.print(".%");
  delay(1000);

  //Se envían los datos del DHT11 a la pantalla LCD
  lcd_1.clear();
  lcd_1.setCursor(0,0);
  lcd_1.print("Temp: ");
  lcd_1.print(celsius);
  lcd_1.print(" C");
  lcd_1.setCursor(0,1);
  lcd_1.print("Fahrenheit:");
  lcd_1.print(fahrenheit);
  lcd_1.print(" F");    
  delay(1500);
  lcd_1.clear();
  lcd_1.setCursor(0,0);
  lcd_1.print("Humedad:");
  lcd_1.print(humedad);
  lcd_1.print(" %");
  lcd_1.setCursor(0,1);
  lcd_1.print("Iluminacion:");
  lcd_1.print(valor_luz);
  lcd_1.print(" Lumenes");
  delay(1500);

  //Se envían los datos del DHT11 al Monitor Serie
  Serial.print(celsius);
  Serial.print(" °C");
  Serial.print("|");
  Serial.print(fahrenheit);
  Serial.print(" °F");
  Serial.print("|");
  Serial.print(humedad);
  Serial.print(" %");
  Serial.print("|");
  Serial.print(valor_luz);
  Serial.print(" lúmenes");
  delay(1500);                
  
  //NOTA: El DHT11 manda datos cada segundo y medio. Si se usa un DHT22, cambiarlo por delay(2000)

//SISTEMA DE ALARMA AL ENTRAR EN LA PUERTA
  int value = 0; //Declaramos una variable llamada value donde guardara el valor de la luz leida por el sensor
  /*value = digitalRead(sensorPin); //el valor va a ser leido por el sensor
  
  digitalWrite(LEDIV,HIGH); //Led verde indica que no hay movimiento y todo esta seguro
  digitalWrite(LEDIR,LOW); //Led Rojo indicara que hay movimiento inesperado
  
  if (value == HIGH) //Aquí indicamos que si el valor esta encendido, en el Serial mostrara que no hay obstaculo y por lo tanto en el noTOne(buzzer) no emitirá un sonido
  {
    Serial.println("Seguro: No hay movimiento");
    noTone(buzzer);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SEGURO: SIN");
    lcd_1.setCursor(0,1);
    lcd_1.print("MOVIMIENTO");
    delay(1000);
  }
  //En caso contrario 
  else
  {
    digitalWrite(LEDIR,HIGH); //El Led Rojo se enciende, indicando que hay movimiento
    digitalWrite(LEDIV,LOW); //El Led Verde se apaga
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("!!ADVERTENCIA!!");
    lcd_1.setCursor(1,1);
    lcd_1.print("HAY MOVIMIENTO");
    delay(1000);
    tone(buzzer,2500); //Suena un tono
    delay(1000); //Durante 150 ms
    tone(buzzer,1500); //Suena otro tono
    delay(1000); //Durante 150 ms
    noTone(buzzer);
    Serial.println("Advertencia: Detectó movimiento"); //Manda un mensaje al Serial donde indica que hay movimiento
  }
  delay(100); //Se actualizara cada 100 ms */

//SISTEMA AUTOMÁTICO DE LUCES EXTERIORES

  Intencidad = (5.0 * pinFoto * 100.0) / 1024.0;  //Para entender esta formula visitar: http://programarfacil.com/podcast/48-sensor-de-temperatura-en-arduino/
  Serial.print(Intencidad);                       //Manda el valor de la luz al Serial
  Serial.println(" lúmenes");                     // junto con un mensaje que indica el valor de la luz
  delay(1000);                                    //Cada segundo se actualizará
  Intencidad = analogRead(pinFoto);               //Foto Resistencia

  if (Intencidad < 50) {
    // digitalWrite(pinLed, HIGH); //Si hay luz ambiente prende el bombillo
    digitalWrite(LEDE1, HIGH);
    digitalWrite(LEDGA, HIGH);
    digitalWrite(LEDE3, HIGH);
    digitalWrite(LEDE4, HIGH);
    //Se manda una pantalla al LCD
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("LED's EXTERIORES");
    lcd_1.setCursor(0, 1);
    lcd_1.print("ENCENDIDOS");
    delay(1000);
  } else {
    // digitalWrite(pinLed, LOW);
    digitalWrite(LEDE1, LOW);  //Si no hay luz ambiente apaga el bombillo
    digitalWrite(LEDGA, LOW);
    digitalWrite(LEDE2, LOW);
    digitalWrite(LEDE3, LOW);
    digitalWrite(LEDE4, LOW);
    //Se manda una pantalla al lcd
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("LED's EXTERIORES");
    lcd_1.setCursor(0, 1);
    lcd_1.print("APAGADOS");
    delay(1000);

//SISTEMA DE LUCES INTERIORES Y SISTEMA DE ABRIR-CERRAR PUERTAS POR SWITCH Y VOZ
if(BT.available())
Mensaje = Serial.read();
{  //Si llega un dato por el puerto BT se envía al monitor Serial
  //Mensaje = BT.read(); //Guarda en la variable “Mensaje” el dato recibido por Bluetooth
  
  Mensaje=BT.read();
  /*Aqui la condicion con la app, indica que las letras Mayúculas son enviadas tanto por el control manual de la app, como con el control por voz, haciendo que se enciendan y/o 
  abra el objeto, de igual forma las Minúsculas indican que se apagan tanto por manual como por voz.*/
  
  if(Mensaje == 'A') 
  {
    Serial.println("Se encendió el Cuarto 1");
    digitalWrite(LEDR1,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ENCENDIO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("CUARTO 1");
    delay(1000); 
  }
  else if(Mensaje == 'a') 
  {
    Serial.println("Se apagó el Cuarto 1");
    digitalWrite(LEDR1,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE APAGO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("CUARTO 1");
    delay(1000);
  }
  else if(Mensaje == 'B') 
  {
    Serial.println("Se encendió el Cuarto 2");
    digitalWrite(LEDR2,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ENCENDIO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("CUARTO 2");    
    delay(1000);
  }
  else if(Mensaje == 'b') 
  {
    Serial.println("Se apagó el Cuarto 2");
    digitalWrite(LEDR2,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE APAGO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("CUARTO 2");
    delay(1000);
  }    
  else if(Mensaje == 'C') 
  {
    Serial.println("Se encendió el Cuarto 3");
    digitalWrite(LEDR3,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ENCENDIO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("CUARTO 3");
    delay(1000);
  }
  else if(Mensaje == 'c') 
  {
    Serial.println("Se apagó el Cuarto 3");
    digitalWrite(LEDR3,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE APAGO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("CUARTO 3");
    delay(1000);
  }
  else if(Mensaje == 'D') 
  {
    Serial.println("Se encendió el Pasillo 1");
    digitalWrite(LEDP1,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ENCENDIO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("PASILLO 1");
    delay(1000);
  }
  else if(Mensaje == 'd') 
  {
    Serial.println("Se apagó el Pasillo 1");
    digitalWrite(LEDP1,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE APAGO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("PASILLO 1");
    delay(1000);
  }    
  else if(Mensaje == 'E') 
  {
    Serial.println("Se encendió el Pasillo 2");
    digitalWrite(LEDP2,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ENCENDIO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("PASILLO 2");
    delay(1000);
  }
  else if(Mensaje == 'e') 
  {
    Serial.println("Se apagó el Pasillo 2");
    digitalWrite(LEDP2,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE APAGO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("PASILLO 2");
    delay(1000);
  }    
  else if(Mensaje == 'F') 
  {
    Serial.println("Se encendió el Pasillo 3");
    digitalWrite(LEDP3,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ENCENDIO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("PASILLO 3");
    delay(1000);
  }
  else if(Mensaje == 'f') 
  {
    Serial.println("Se apagó el Pasillo 3");
    digitalWrite(LEDP3,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE APAGO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("PASILLO 3");
    delay(1000);
  }    
  else if(Mensaje == 'G') 
  {    
    Serial.println("Se encendió el Baño");
    digitalWrite(LEDBO,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ENCENDIO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("SANITARIO");
    delay(1000);
  }
  else if(Mensaje == 'g') 
  {
    Serial.println("Se apagó el Baño");
    digitalWrite(LEDBO,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE APAGO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("SANITARIO");
    delay(1000);
  }    
  else if(Mensaje == 'H') 
  {
    Serial.println("Se encendió la Sala");
    digitalWrite(LEDSA,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ENCENDIO LA");
    lcd_1.setCursor(0,1);
    lcd_1.print("SALA");
    delay(1000);
  }
  else if(Mensaje == 'h') 
  {
    Serial.println("Se apagó la Sala");
    digitalWrite(LEDSA,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE APAGO LA");
    lcd_1.setCursor(0,1);
    lcd_1.print("SALA");
    delay(1000);
  }    
  else if(Mensaje == 'I') 
  {
    Serial.println("Se encendió el Comedor");
    digitalWrite(LEDCO,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ENCENDIO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("COMEDOR");
    delay(1000);
  }
  else if(Mensaje == 'i') 
  {
    Serial.println("Se apagó el Comedor");
    digitalWrite(LEDCO,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE APAGO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("COMEDOR");
    delay(1000);
  }
  else if(Mensaje == 'J') 
  {
    Serial.println("Se encendió la Cocina");
    digitalWrite(LEDCA,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ENCENDIO LA");
    lcd_1.setCursor(0,1);
    lcd_1.print("COCINA");
    delay(1000);
  }
  else if(Mensaje == 'j') 
  {
    Serial.println("Se apagó la Cocina");
    digitalWrite(LEDCA,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE APAGO LA");
    lcd_1.setCursor(0,1);
    lcd_1.print("COCINA");
    delay(1000);
  }
  else if(Mensaje == 'K') //Control Garaje ----------------------------------
  {
    Serial.println("Se encendió el Garaje");
    digitalWrite(LEDGA,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ENCENDIO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("GARAJE");
    delay(1000);
  }
  else if(Mensaje == 'k') 
  {
    Serial.println("Se apagó el Garaje");
    digitalWrite(LEDGA,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE APAGO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("GARAJE");
    delay(1000);
  }
  else if(Mensaje == 'L') 
  {
    Serial.println("Se encendieron los Cuartos");
    digitalWrite(LEDR1,HIGH);
    digitalWrite(LEDR2,HIGH);
    digitalWrite(LEDR3,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ENCENDIERON");
    lcd_1.setCursor(0,1);
    lcd_1.print("TODOS LOS CUARTOS");
    delay(1000);
  }  
  else if(Mensaje == 'l') 
  {
    Serial.println("Se apagaron los Cuartos");
    digitalWrite(LEDR1,LOW);
    digitalWrite(LEDR2,LOW);
    digitalWrite(LEDR3,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE APAGARON");
    lcd_1.setCursor(0,1);
    lcd_1.print("TODOS LOS CUARTOS");
    delay(1000);
  }
  else if(Mensaje == 'M') 
  {
    Serial.println("Se encendieron los Pasillos");
    digitalWrite(LEDP1,HIGH);
    digitalWrite(LEDP2,HIGH);
    digitalWrite(LEDP3,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ENCENDIERON");
    lcd_1.setCursor(0,1);
    lcd_1.print("TODOS LOS PASILLOS");
    delay(1000);
  }
  else if(Mensaje == 'm') 
  {
    Serial.println("Se apagaron los Pasillos");
    digitalWrite(LEDP1,LOW);
    digitalWrite(LEDP2,LOW);
    digitalWrite(LEDP3,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE APAGARON");
    lcd_1.setCursor(0,1);
    lcd_1.print("TODOS LOS PASILLOS");
    delay(1000);
  }
  else if(Mensaje == 'N') 
  {
    Serial.println("Se encendió toda la Casa");
    digitalWrite(LEDR1,HIGH);
    digitalWrite(LEDR2,HIGH);
    digitalWrite(LEDR3,HIGH);
    digitalWrite(LEDP1,HIGH);
    digitalWrite(LEDP2,HIGH);
    digitalWrite(LEDP3,HIGH);
    digitalWrite(LEDBO,HIGH);
    digitalWrite(LEDSA,HIGH);
    digitalWrite(LEDCO,HIGH);
    digitalWrite(LEDCA,HIGH);
    digitalWrite(LEDGA,HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ENCENDIO");
    lcd_1.setCursor(0,1);
    lcd_1.print("TODA LA CASA");
    delay(1000);
  }
  else if(Mensaje == 'n') 
  {
    Serial.println("Se apagó toda la Casa");
    digitalWrite(LEDR1,LOW);
    digitalWrite(LEDR2,LOW);
    digitalWrite(LEDR3,LOW);
    digitalWrite(LEDP1,LOW);
    digitalWrite(LEDP2,LOW);
    digitalWrite(LEDP3,LOW);
    digitalWrite(LEDBO,LOW);
    digitalWrite(LEDSA,LOW);
    digitalWrite(LEDCO,LOW);
    digitalWrite(LEDCA,LOW);
    digitalWrite(LEDGA,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE APAGO");
    lcd_1.setCursor(0,1);
    lcd_1.print("TODA LA CASA");
    delay(1000);
  }
  
  //ABRIR O CERRAR PUERTAS POR VOZ O SWITCHES
  //Garaje 
  else if(Mensaje == 'O') 
  {
    Serial.println("Se abrio Garaje por Switch o Voz");
    digitalWrite(LEDVG,HIGH);
    digitalWrite(LEDRG,LOW);
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ABRIO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("GARAJE");
    delay(1000);    
    tone(buzzer,600);
    delay(1000);
    noTone(buzzer);
    garage.write(0);
    delay(8000);
    tone(buzzer,500);
    delay(2000);
    noTone(buzzer);
    garage.write(100);
    delay(1000);
  }
    /*No hay necesidad de poner un apartado de que se cierre la puerta manualmente, ya que el loop en la parte donde lee la distancia del ultrasónico, la puerta del garaje
  se cierra automaticamente, pero lo pondre por si en dado caso, el dato que se envia al Bluetooth no llega, con la voz se puede volver activar.*/
  else if(Mensaje == 'o') 
  {
    Serial.println("Se cerró Garaje por Switch o Voz");
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE CERRO EL");
    lcd_1.setCursor(0,1);
    lcd_1.print("GARAJE");
    delay(1000);
    noTone(buzzer);
    digitalWrite(LEDVG,LOW);
    digitalWrite(LEDRG,HIGH);
    garage.write(0);   
  }
 /*else if(Mensaje == 'p') 
  {
    Serial.println("Se cerró la Puerta 0");
    lcd_1.clear();
    lcd_1.setCursor(5,1);
    lcd_1.print("SE CERRO LA");
    lcd_1.setCursor(7,2);
    lcd_1.print("PUERTA");
    puerta.write(0);
    //delay(2000);
  }
  else if(Mensaje == 'P') 
  {
    Serial.println("Se abrió la Puerta 120 grados");
    lcd_1.clear();
    lcd_1.setCursor(5,1);
    lcd_1.print("SE ABRIO LA");
    lcd_1.setCursor(7,2);
    lcd_1.print("PUERTA");
    delay(1000);
    puerta.write(120);
  }*/
  else if(Mensaje == 'Q') 
  {
    Serial.println("Se abrió la Ventana");
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE ABRIO LA");
    lcd_1.setCursor(0,1);
    lcd_1.print("VENTANA");
    delay(1000);
    ventana.write(0);
    //delay(5000);
  }
  else if(Mensaje == 'q') 
  {
    Serial.println("Se cerró la Ventana");
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("SE CERRO LA");
    lcd_1.setCursor(2,1);
    lcd_1.print("VENTANA");
    delay(1000);
    ventana.write(90);
    //delay(5000);
  }
  else if(Mensaje == 'R') 
  {
    Serial.println("Julian es tu perra 4ever");
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("JULIAN ES TU");
    lcd_1.setCursor(2,1);
    lcd_1.print("PERRA");
    delay(2000);
   // ventana.write(0);
    //delay(5000);
  }
  else if(Mensaje == 'S') 
  {
    Serial.println("Mejor me apago!");
    lcd_1.clear();
    lcd_1.setCursor(0,0);
    lcd_1.print("ERROR!! GARLY");
    lcd_1.setCursor(2,1);
    lcd_1.print(" ES MUY MANCO");
    delay(200);
    //ventana.write(90);
    //delay(5000);
  }
  delay(100);
 }
}
}