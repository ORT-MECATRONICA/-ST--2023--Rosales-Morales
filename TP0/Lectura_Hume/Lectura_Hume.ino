#include <Adafruit_Sensor.h>
#include <dummy.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

float h;
float t;

void setup() {  
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  dhtFunc();
  Serial.println(t);
}



void dhtFunc(){
 
    h = dht.readHumidity(); //Leemos la Humedad
    t = dht.readTemperature(); //Leemos la temperatura en grados Celsius
}
