//Por Joaquin R - Santi M
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

#define BOTON_MINUTOS 34
#define BOTON_HORAS 35

#define SET 0
#define FIN 1

#define PANTALLA1 0
#define DEFINIR_HORA 1

DHT dht(DHTPIN, DHTTYPE);

float temperaturaActual;
int funcionamiento;
float RELOJ;
float temperaturaAntes;

int lecturaBOTON_MINUTOS;
int lecturaBOTON_HORAS;

int estadoBoton;
int estadoBOTON_MINUTOS;
int estadoBOTON_HORAS;
int cambioEstados;
int definirRELOJ; 
int flagTemperatura;


int ms;
int seg;
int MINUTOS;
int HORAS;

void setup() {
  pinMode(BOTON_MINUTOS, INPUT_PULLUP);
  pinMode(BOTON_HORAS, INPUT_PULLUP);

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

  ms = millis();
}

void loop() {
  temperaturaAntes = temperaturaActual;
  temperaturaActual = dht.readTemperature();

  if (flagTemperatura == 0) {
    temperaturaAntes = temperaturaActual;
    flagTemperatura = 1;
  }

  lecturaBOTON_MINUTOS = digitalRead(BOTON_MINUTOS);
  lecturaBOTON_HORAS = digitalRead(BOTON_HORAS);

  switch (funcionamiento) {

    case PANTALLA1:
      
      if (temperaturaAntes != temperaturaActual) {
        display.clearDisplay();
      }

      display.setCursor(0, 25);
      display.print("Temp actual: ");
      display.println(temperaturaActual);
      display.display();

      if (millis() - ms  >= 1000) {
        seg = seg + 1;
        ms = millis();
      }

      if (seg >= 60) {
        display.clearDisplay();
        MINUTOS = MINUTOS + 1;
        seg = 0;
      }

      if (MINUTOS >= 60) {
        display.clearDisplay();
        HORAS = HORAS + 1;
        MINUTOS = 0;
      }

      if (HORAS >= 24) {
        HORAS = 0;
      }

      if (definirRELOJ == SET) {
        display.setCursor(0, 45);
        display.print("Elija horario deseado");
        display.display();
      }

      else {
        display.setCursor(0, 45);
        display.print("Hora: ");
        display.print(HORAS);
        display.print(":");
        display.print(MINUTOS);
        display.display();
      }

      if (lecturaBOTON_MINUTOS == LOW && lecturaBOTON_HORAS == LOW ) {
        cambioEstados = HIGH;
      }

      if (lecturaBOTON_MINUTOS == HIGH && lecturaBOTON_HORAS == HIGH && cambioEstados == HIGH) {
        cambioEstados = LOW;
        funcionamiento = 1;
      }

      break;

    case DEFINIR_HORA:

      definirRELOJ = FIN;

      display.clearDisplay();

      display.setCursor(0, 25);
      display.println("Hora elegida: ");
      display.print(HORAS);
      display.print(":");
      display.print(MINUTOS);
      display.display();


      if (lecturaBOTON_MINUTOS == LOW && lecturaBOTON_HORAS == LOW ) {
        cambioEstados = HIGH;
      }

      if (lecturaBOTON_MINUTOS == HIGH && lecturaBOTON_HORAS == HIGH && cambioEstados == HIGH) {
        cambioEstados = LOW;
        funcionamiento = 0;
        display.clearDisplay();
        break;

      }

      if (lecturaBOTON_MINUTOS == LOW && lecturaBOTON_HORAS == HIGH && cambioEstados == LOW) {
        estadoBOTON_MINUTOS = HIGH;
      }

      if (lecturaBOTON_MINUTOS == HIGH && estadoBOTON_MINUTOS == HIGH && cambioEstados == LOW) {
        MINUTOS += 1;
        estadoBOTON_MINUTOS = LOW;
      }

      if (lecturaBOTON_HORAS == LOW && lecturaBOTON_MINUTOS == HIGH && cambioEstados == LOW) {
        estadoBOTON_HORAS = HIGH;
      }

      if (lecturaBOTON_HORAS == HIGH && estadoBOTON_HORAS == HIGH && cambioEstados == LOW) {
        HORAS += 1;
        estadoBOTON_HORAS = LOW;
      }

      if (HORAS == 24) {
        HORAS = 0;
      }

      if (MINUTOS == 60) {
        MINUTOS = 0;
      }


      break;


  }
}
