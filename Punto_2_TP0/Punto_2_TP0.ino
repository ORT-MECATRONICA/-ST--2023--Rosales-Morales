
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>     // libreria para bus I2C
#include <Adafruit_GFX.h>   // libreria para pantallas graficas
#include <Adafruit_SSD1306.h>   // libreria para controlador SSD1306
#include <Adafruit_Sensor.h>
#include <dummy.h>

#define ANCHO 128     
#define ALTO 64       

#define OLED_RESET 4      
Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);  // crea objeto

#define DHTPIN 23     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

float h;
float t;

void setup() {
  Wire.begin();         // inicializa bus I2C
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // inicializa pantalla con direccion 0x3C
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  dhtFunc();
  Serial.println(t);
  float temperatura = dht.readTemperature();
  oled.clearDisplay();      // limpia pantalla
  oled.setTextColor(WHITE);   
  oled.setCursor(0, 20);     // ubica cursor en inicio de coordenadas 0,0
  oled.setTextSize(3);      
  oled.print(temperatura);  // escribe en pantalla el texto
  oled.display();     // muestra en pantalla todo lo establecido anteriormente
}

void dhtFunc(){
 
    h = dht.readHumidity(); //Leemos la Humedad
    t = dht.readTemperature(); //Leemos la temperatura en grados Celsius
}
