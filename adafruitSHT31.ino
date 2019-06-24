#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include "Adafruit_SHT31.h"
#define Vin 2
Adafruit_SHT31 sht31 = Adafruit_SHT31();
#include <ESP8266WebServer.h>
unsigned int I2CTimer = 0;
unsigned int I2CInterval = 2000;

//static Ip for AP
IPAddress ap_local_IP(192,168,1,120);
IPAddress ap_gateway(192,168,1,254);
IPAddress ap_subnet(255,255,255,0);

const char APssid[] = "admin";
const char APpass[] = "admin";

float tempC= 25.0;
float humid= 60.0;
float tempF = 0.0;





     ESP8266WebServer server(80);
void setup() {
  Serial.begin(9600);
   pinMode(Vin, OUTPUT);
  delay(100);
  digitalWrite(Vin,HIGH);
  
  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
   reconnectAPWiFi();
   server.on("/", onHandleDataRoot);
   server.begin();
  
}


void loop() {
  if(millis()- I2CTimer >= I2CInterval){
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  float tf = (t*1.8)+32;
  if (! isnan(t)) {  // check if 'is not a number'
    Serial.print("Temp *C = "); Serial.println(t);
  } else { 
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    Serial.print("Hum. % = "); Serial.println(h);
  } else { 
    Serial.println("Failed to read humidity");
  }
  Serial.println();
  tempC = t;
  humid = h;
  tempF = tf;
  I2CTimer = millis();
  }
  server.handleClient();
}

void onHandleDataRoot(){
       String PAGE2 = 
     "<!DOCTYPE html>"
     "<html>"
     "<head>"
     "<meta charset=\"utf-8\">"
     "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0\">"
     "<title> My first HTML page</title>"
     "</head>"
     "<body style= \"background-color:#EDEEF2\">"
     "<h1 style= \"color:blue\">Mausam Purvanuman</h1>"
     "<h3 style= \"font-family:verdana; color:blue\">Tapman in Celcius:</h3>"
     "<p style=\"font-size:160%; font-family:Lucida Console\">"+String(tempC,2)+
     "</p>"
     "<h3 style= \"font-family:verdana; color:blue\">Tapman in Fahrenheit:</h3>"
     "<p style=\"font-size:160%; font-family:Lucida Console\">"+String(tempF,2)+
     "</p>"
     "<h3 style= \"font-family:verdana; color:blue\">Nami:</h3>"
     "<p style=\"font-size:160%; font-family:Lucida Console\">"+String(humid,2)+
     "</p>"
     "</body>"
     "</html>";
       server.send(200, "text/html", PAGE2);
    }

    
void reconnectAPWiFi(){
    WiFi.mode(WIFI_AP);
    delay(100);
    Serial.println(WiFi.softAP(APssid)?"WiFi Setup":"WiFi not setup") ;
    boolean status = WiFi.softAPConfig(ap_local_IP, ap_gateway, ap_subnet);
    if(status ==true){
        Serial.print("Setting soft-AP ... ");   
        Serial.print("connected to:\t");
        //IPAddress myIP = WiFi.softAPIP();
        Serial.println(WiFi.softAPIP());      
      }
}    
