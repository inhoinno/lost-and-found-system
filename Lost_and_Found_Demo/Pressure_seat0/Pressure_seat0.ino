#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketClient.h>

#ifndef STASSID
#define STASSID "raspi_SSongJeong"
#define STAPSK  "32152332"
#endif

//WiFi setup
String DEVICE_NAME = "Pressure 0 : ";
int s_status = WL_IDLE_STATUS;
const char* ssid = STASSID;
const char* password = STAPSK;
const int Port = 9999;
char path [] = "/";
char host [] = "192.168.4.1";

int FSR_PIN = A0;
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 3230.0; // Measured resistance of 3.3k resistor

WebSocketClient webSocketClient;
WiFiClient client;
void _WiFi_setup(){
   WiFi.mode(WIFI_STA);
  s_status = WiFi.begin(ssid, password);
  Serial.println("");
Serial.println("  WiFi Begin() Done.");

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

 
 // 서버에 연결
  /**************************************/
  /*****  Server Connection Setup  *****/

    if (client.connect(host, Port)) {
        Serial.println("Connected");
    } else {
        Serial.println("Connection failed.");
    }
    delay(1000); 

    webSocketClient.path = path;
    webSocketClient.host = host;
 
    if (webSocketClient.handshake(client)) {
        Serial.println("Handshake successful");
    } else {
        Serial.println("Handshake failed.");
    }
      
  }
void setup() {
  // put your setup code here, to run once:

    
  Serial.begin(115200);
  Serial.println("Pressure Setup Start...");
  /**************************************/
  /***********Pressure Sensor Set***********/
  
  pinMode(FSR_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  /*********Pressure Sensor END************/
  /**************************************/
Serial.println("Pressure Setup Done.");
  /**************************************/
  /**********   WiFi Setup      *********/
  Serial.println("WiFi Setup Start...");
  Serial.println("  WiFi Begin() Start...");
    // ERROR : ets Jan  8 2013,rst cause:4, boot mode:(3,6)
  WiFi.mode(WIFI_STA);
  s_status = WiFi.begin(ssid, password);
  Serial.println("");
Serial.println("  WiFi Begin() Done.");

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

 
 // 서버에 연결
  /**************************************/
  /*****  Server Connection Setup  *****/

    if (client.connect(host, Port)) {
        Serial.println("Connected");
    } else {
        Serial.println("Connection failed.");
    }
    delay(1000); 

    webSocketClient.path = path;
    webSocketClient.host = host;
 
    if (webSocketClient.handshake(client)) {
        Serial.println("Handshake successful");
    } else {
        Serial.println("Handshake failed.");
    }
      
  
  //printCurrentNet();
  //printWiFiData();
}



void loop() {
   delay(100);
  String msg = "Lost Detected";  
 int fsrADC = analogRead(FSR_PIN);
  // If the FSR has no pressure, the resistance will be
  // near infinite. So the voltage should be near 0.
  if (fsrADC != 0) // If the analog reading is non-zero
  {
    // Use ADC reading to calculate voltage:
    float fsrV = fsrADC * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    float fsrR = R_DIV * (VCC / fsrV - 1.0);
    Serial.println("Resistance: " + String(fsrR) + " ohms");
    // Guesstimate force based on slopes in figure 3 of
    // FSR datasheet:
    float force;
    float fsrG = 1.0 / fsrR; // Calculate conductance
    // Break parabolic curve down into two linear slopes:
    
    Serial.println("!!" + String(fsrADC));
    if (fsrADC > 8){
      digitalWrite(LED_BUILTIN, LOW);  
      if ((client.connected())){
    //연결 완료
    //데이터 전송
        webSocketClient.sendData(DEVICE_NAME +msg);    
        Serial.println("Detected, Send.");
      }else{
        Serial.print(".");
       _WiFi_setup();  //A Line to avoid webSocket Error 1006 : "connection failed with no reason"
      //When Connection is Failed Due to websocket Network error, Try WiFi.begin again 
              //return;  
      }
    }
    else{
      digitalWrite(LED_BUILTIN, HIGH);      
      webSocketClient.sendData("P.");  }  

 
    if (fsrR <= 600) 
    {
      force = (fsrG - 0.00075) / 0.00000032639;
     
    }
    else{
      force =  fsrG / 0.000000642857;

     }
    Serial.println("Force: " + String(force) + " g");
    //Serial.println();
    
   
    
    //delay(1000);
  }
}
