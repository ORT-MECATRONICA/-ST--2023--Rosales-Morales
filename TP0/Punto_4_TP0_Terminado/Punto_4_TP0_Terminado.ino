//Por Joaquin R y Santiago M
//Curso: 5LA 2023

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 23
#define DHTTYPE DHT11
#define TEMP 27

#define BOTON_RESTA 34
#define BOTON_SUMA 35

#define SET 0
#define FIN 1

#define TEMPERATURA 0
#define UMBRAL 1

DHT dht(DHTPIN, DHTTYPE);

float temperaturaActual;
int funcionamiento;
float umbral;
float temperaturaAntes;

int lecturaBOTON_RESTA;
int lecturaBOTON_SUMA;
int estadoBoton;
int estadoBOTON_RESTA;
int estadoBOTON_SUMA;
int cambioEstados;
int definirUmbral; //se hace solo una vez
int flagTemperatura;

void setup() {

  pinMode(BOTON_RESTA, INPUT_PULLUP);
  pinMode(BOTON_SUMA, INPUT_PULLUP);

  pinMode(TEMP, INPUT);

  Serial.begin(115200);
  dht.begin();


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);


}

void loop() {
  temperaturaAntes = temperaturaActual;
  temperaturaActual = dht.readTemperature();
  if (flagTemperatura == 0) {
    temperaturaAntes = temperaturaActual;
    flagTemperatura = 1;
  }
  lecturaBOTON_RESTA= digitalRead(BOTON_RESTA);
  lecturaBOTON_SUMA = digitalRead(BOTON_SUMA);

  switch (funcionamiento) {
    case TEMPERATURA:
      if (temperaturaAntes != temperaturaActual) {
        display.clearDisplay();

      }
      display.setCursor(0, 25);
      display.print("Temp actual: ");
      display.println(temperaturaActual);
      Serial.println(temperaturaActual);
      display.display();
      
      if (definirUmbral == SET) {
        display.setCursor(0, 45);
        display.print("Elija umbral deseado");
        display.display();
      }
      else {

        display.setCursor(0, 45);
        display.print("Temp umbral: ");
        display.print(umbral);
        Serial.println(umbral);
        display.display();
      }

      if (lecturaBOTON_RESTA == LOW && lecturaBOTON_SUMA == LOW ) {
        cambioEstados = HIGH;
      }

      if (lecturaBOTON_RESTA == HIGH && lecturaBOTON_SUMA == HIGH && cambioEstados == HIGH) {
        cambioEstados = LOW;
        funcionamiento = 1;
      }

      break;

    case UMBRAL:

      definirUmbral = FIN;

      display.clearDisplay();

      display.setCursor(0, 25);
      display.println("Temperatura umbral");
      display.print(umbral); 
      Serial.println(umbral);
      display.display();


      if (lecturaBOTON_RESTA == LOW && lecturaBOTON_SUMA == LOW ) {
        cambioEstados = HIGH;
      }

      if (lecturaBOTON_RESTA == HIGH && lecturaBOTON_SUMA == HIGH && cambioEstados == HIGH) {
        cambioEstados = LOW;
        funcionamiento = 0;
        display.clearDisplay();
        break;

      }

      if (lecturaBOTON_RESTA == LOW && lecturaBOTON_SUMA == HIGH && cambioEstados == LOW) {
        estadoBOTON_RESTA = HIGH;
      }

      if (lecturaBOTON_RESTA == LOW && estadoBOTON_RESTA == HIGH && cambioEstados == LOW) {
        umbral -= 1;
        estadoBOTON_RESTA = LOW;
      }

      if (lecturaBOTON_SUMA == LOW && lecturaBOTON_RESTA == HIGH && cambioEstados == LOW) {
        estadoBOTON_SUMA = HIGH;
      }

      if (lecturaBOTON_SUMA == HIGH && estadoBOTON_SUMA == HIGH && cambioEstados == LOW) {
        umbral += 1;
        estadoBOTON_SUMA = LOW;
      }

      break;



  }
}
