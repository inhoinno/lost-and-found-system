#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#ifndef STASSID
#define STASSID "raspi_inhoinno"
#define STAPSK  "32152332"
#endif
int s_status = WL_IDLE_STATUS;
const char* ssid = STASSID;
const char* password = STAPSK;
const int led = 13;
int green = 12;
void setup() {
  // put your setup code here, to run once:
  pinMode(green,OUTPUT); //Green LED 
  pinMode(LED_BUILTIN, OUTPUT); //WIFI LED
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  s_status = WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   //s_status =  WiFi.begin(ssid, password);
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("You're connect to the network \"");
  Serial.print(ssid);
  Serial.println("\"");

  //printCurrentNet();
  //printWiFiData();
}



void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(green, HIGH); 
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  delay(1000);
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  digitalWrite(green, LOW);
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(1000);                      // Wait for two seconds (to demonstrate the active low LED)

  WiFiClient client = WiFiClient();
  const int Port = 80;

  if ((!client.connect("192.168.4.1",Port))){
    Serial.print(".");
    return;
    }
    client.print("Sensor 1\n");

    
    

  }
