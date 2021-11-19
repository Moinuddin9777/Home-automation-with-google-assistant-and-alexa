// Uncomment the following line to enable serial debug output
#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
       #define DEBUG_ESP_PORT Serial
       #define NODEBUG_WEBSOCKETS
       #define NDEBUG
#endif 

#include <Arduino.h>
#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif

#include "SinricPro.h"
#include "SinricProLight.h"

#define WIFI_SSID         "MOs wifi"    
#define WIFI_PASS         "59732419"
#define APP_KEY           "e8480bbe-12eb-466c-8b36-442fa35ef9a7"      // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET        "7d1445dd-19c9-4e1f-9d46-bebca078f417-287d04d3-795c-4421-8622-0b9f10d0f189"   // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"
#define blueLight_ID      "616733aabf77364fa09efca3"    // Should look like "5dc1564130xxxxxxxxxxxxxx"
#define greenLight_ID     "6167346abf77364fa09efcb4"
#define BAUD_RATE         9600                // Change baudrate to your need

#define BAUD_RATE         115200                // Change baudrate to your need

const int blueLight=D5;
const int greenlight=D6;

bool onPowerState(const String &deviceId, bool &state) {
    if(deviceId=="616733aabf77364fa09efca3"){
       Serial.printf("Device %s power turned %s \r\n", deviceId.c_str(), state?"on":"off");
      if(state){
        digitalWrite(blueLight,HIGH);
        Serial.println("BLUE LIGHT TURNED ON");
      }
      else{
        digitalWrite(blueLight,LOW);
        }
    }
    else if(deviceId=="6167346abf77364fa09efcb4"){
       Serial.printf("Device %s power turned %s \r\n", deviceId.c_str(), state?"on":"off");
      if(state){
        digitalWrite(greenlight,HIGH);
      }
      else{
        digitalWrite(greenlight,LOW);
        }
    }
  return true; // request handled properly
}

bool onBrightness(const String &deviceId, int &brightness) {
  if(deviceId =="616733aabf77364fa09efca3"){
    Serial.printf("Device %s brightness level changed to %d\r\n", deviceId.c_str(), brightness);
    int mappedIntensity = map(brightness, 0, 100, 0, 255);
    analogWrite(blueLight,mappedIntensity);
  }
  else if(deviceId=="6167346abf77364fa09efcb4"){
     Serial.printf("Device %s brightness level changed to %d\r\n", deviceId.c_str(), brightness);
    int mappedIntensity = map(brightness, 0, 100, 0, 255);
     analogWrite(greenlight,mappedIntensity);
  } 
  return true;
}

void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  IPAddress localIP = WiFi.localIP();
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %d.%d.%d.%d\r\n", localIP[0], localIP[1], localIP[2], localIP[3]);
}

void setupSinricPro() {
  // get a new Light device from SinricPro
  SinricProLight &myLight1 = SinricPro[blueLight_ID];
  SinricProLight &myLight2 = SinricPro[greenLight_ID];

  // set callback function to RED LIGHT
  myLight1.onPowerState(onPowerState);
  myLight1.onBrightness(onBrightness);

    // set callback function to RED LIGHT
  myLight2.onPowerState(onPowerState);
  myLight2.onBrightness(onBrightness);

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  pinMode(blueLight,OUTPUT);
  pinMode(greenlight,OUTPUT);
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}
