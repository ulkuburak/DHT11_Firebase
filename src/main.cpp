#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>
#include "DHTesp.h"
#include <Adafruit_Sensor.h>

#define WIFI_SSID "POCO X3 NFC"
#define WIFI_PASSWORD "a********789"
#define DATABASE_URL "https://***************.firebaseio.com/"
#define API_KEY "AIzaSyDyof*************GMY4eoib5gjs"

#define DHTpin 33    // G33 of ESP32 DevKit
#define DHT_TYPE DHTesp::DHT11
DHTesp dht;
FirebaseData firebaseData;
void setup()
{
  Firebase.begin(DATABASE_URL, API_KEY);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  Serial.begin(9600);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
  

  dht.setup(DHTpin, DHT_TYPE);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    break;
  }

  Serial.println();
  Serial.print("Connected to Wi-Fi network with IP address: ");
  Serial.println(WiFi.localIP());

  //Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, wifiClient);
  //Firebase.begin(DATABASE_URL, API_KEY, wifiClient);
  
  // Autodetect is not working reliable, don't use the following line
  // dht.setup(17);

  // use this instead: 
  dht.setup(DHTpin, DHTesp::DHT11); //for DHT11 Connect DHT sensor to GPIO 17
  //dht.setup(DHTpin, DHTesp::DHT22); //for DHT22 Connect DHT sensor to GPIO 17
}

void loop()
{
  
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);
    
FirebaseJson json;
json.set("/T", temperature);
json.set("/H", humidity);
Firebase.updateNode(firebaseData, "/sensors", json);


   // wait for 10 seconds before reading the sensor again
}
