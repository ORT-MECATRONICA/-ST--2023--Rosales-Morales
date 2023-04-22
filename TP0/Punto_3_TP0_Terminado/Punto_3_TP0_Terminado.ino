//Por Joaquin R y Santiago M
//Curso: 5LA 2023

#include <Wire.h>     // libreria para bus I2C
#include <Adafruit_GFX.h>   // libreria para pantallas graficas
#include <Adafruit_SSD1306.h>   // libreria para controlador SSD1306

#define ANCHO 128     // reemplaza ocurrencia de ANCHO por 128
#define ALTO 64       // reemplaza ocurrencia de ALTO por 64

#define OLED_RESET 4      // necesario por la libreria pero no usado
Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);  // crea objeto

#include "DHT.h"

#define DHTPIN 23     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
#define RELAY   18
#define UMBRAL  31

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Wire.begin();         // inicializa bus I2C
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // inicializa pantalla con direccion 0x3C
  dht.begin();
  pinMode(RELAY, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  float t = dht.readTemperature();
  oled.clearDisplay();      // limpia pantalla
  oled.setTextColor(WHITE);   // establece color al unico disponible (pantalla monocromo)
  oled.setCursor(0, 20);     // ubica cursor en inicio de coordenadas 0,0
  oled.setTextSize(3);      // establece tamano de texto en 1
  oled.print(t);  // escribe en pantalla el texto
  Serial.println(t);
  oled.display();     // muestra en pantalla todo lo establecido anteriormente

  if(t > UMBRAL){
    digitalWrite(RELAY, HIGH);
  }else{
    digitalWrite(RELAY, LOW);
  }
}
