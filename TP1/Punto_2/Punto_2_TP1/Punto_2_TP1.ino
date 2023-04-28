//Por Joaquin R y Santiago M
//Curso: 5LA 2023

#include <Arduino.h>
#include <ESP32Time.h>
#include "time.h"
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

#define DHTPIN 23
#define DHTTYPE DHT11
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define BOTON_1 34
#define BOTON_2 35
#define RELE 26

int modosPantalla;
int lecturaBoton1;
int lecturaBoton2;
int tprevio;
float t = 0;
int GMT = -3;

#define MODO_INIT 0
#define MODO_1 1
#define MODO_2 2
#define MODO_ESPERA1 3
#define MODO_ESPERA2 4
#define SUBIR_GMT 5
#define BAJAR_GMT 6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);

void pedir_hora(void);
void setup_rtc_ntp(void);

struct tm timeinfo;
ESP32Time rtc;

/// time
long unsigned int timestamp; // hora
const char *ntpServer = "south-america.pool.ntp.org";
long gmtOffset_sec = -10800;
const int daylightOffset_sec = 0;

const char* ssid = "ORT-IoT";
const char* password = "OrtIOTnew22$2";

void setup() {
  Serial.begin(115200);
  pinMode(RELE, OUTPUT);
  pinMode(BOTON_1, INPUT_PULLUP);
  pinMode(BOTON_2, INPUT_PULLUP);
  dht.begin();
  modosPantalla = MODO_INIT;
  Serial.println(F("Programa Iniciado"));

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();
  Serial.println("Connecting to Wi-Fi...");
  initWiFi();
  setup_rtc_ntp();
}

void loop() {
  pedir_hora();
  //delay (2000);

  tprevio = t;
  t = dht.readTemperature();

  Serial.print(F("% Temperatura: "));
  Serial.print(t);

  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  lecturaBoton1 = digitalRead(BOTON_1);
  lecturaBoton2 = digitalRead(BOTON_2);

  switch (modosPantalla) {

    case MODO_INIT:

      modosPantalla = MODO_1;

      break;

    case MODO_1:

      //Serial.print(F("% segundos: "));
      //Serial.print(seg);
      Serial.println("modo 1");
      if (t != tprevio) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        // Display temperatura
        display.setCursor(0, 10);
        display.println(t);
        // Display hora
        display.setCursor(0, 40);
        display.println(&timeinfo, "%H:%M");
        display.display();
      }

      if (lecturaBoton1 == LOW && lecturaBoton2 == LOW) {
        modosPantalla = MODO_ESPERA1;
      }

      break;

    case MODO_2:
      Serial.println("modo 2");
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      // Display hora
      display.setCursor(0, 40);
      display.println("GMT:");
      display.setCursor(25, 40);
      display.println(GMT);
      display.display();



      if (lecturaBoton1 == LOW) {
        display.clearDisplay();
        modosPantalla = SUBIR_GMT;
      }

      if (lecturaBoton2 == LOW) {
        display.clearDisplay();
        modosPantalla = BAJAR_GMT;
      }

      if (lecturaBoton1 == LOW && lecturaBoton2 == LOW) {
        modosPantalla = MODO_ESPERA2;
      }

      break;

    case MODO_ESPERA1:
      Serial.println("modo ESPERA 1");
      if (lecturaBoton1 == HIGH && lecturaBoton2 == HIGH) {
        display.clearDisplay();
        modosPantalla = MODO_2;
      }

      break;

    case MODO_ESPERA2:
      Serial.println("modo ESPERA 2");
      if (lecturaBoton1 == HIGH && lecturaBoton2 == HIGH) {
        display.clearDisplay();
        modosPantalla = MODO_1;
      }

      break;

    case SUBIR_GMT:
      Serial.println("Subir gmt");
      if (lecturaBoton1 == HIGH) {
        if (gmtOffset_sec < 43200) {
          gmtOffset_sec += 3600;
          GMT += 1;
        }
        display.clearDisplay();
        setup_rtc_ntp();
        modosPantalla = MODO_2;
      }


      if (lecturaBoton2 == LOW) {
        modosPantalla = MODO_ESPERA2;
      }

      break;

    case BAJAR_GMT:
      Serial.println("Bajar gmt");
      if (lecturaBoton2 == HIGH) {
        if (gmtOffset_sec > -43200) {
          gmtOffset_sec -= 3600;
          GMT -= 1;
        }
        display.clearDisplay();
        setup_rtc_ntp();
        modosPantalla = MODO_2;
      }

      if (lecturaBoton1 == LOW) {
        modosPantalla = MODO_ESPERA2;
      }

      break;

  }
}

// Initialize WiFi
void initWiFi() {
  WiFi.begin(ssid , password );
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setup_rtc_ntp(void)
{
  // init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  timestamp = time(NULL);
  rtc.setTime(timestamp + gmtOffset_sec);
}

void pedir_hora(void)
{
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("veo la hora del rtc interno ");
    timestamp = rtc.getEpoch() - gmtOffset_sec;
    timeinfo = rtc.getTimeStruct();
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  }
  else
  {
    Serial.print("NTP Time:");
    timestamp = time(NULL);
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  }

  return;
}
