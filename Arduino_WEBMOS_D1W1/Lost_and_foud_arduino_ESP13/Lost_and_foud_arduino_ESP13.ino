#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#ifndef STASSID
#define STASSID "raspi_inhoinno"
#define STAPSK  "32152332"
#endif

//WiFi setup
int s_status = WL_IDLE_STATUS;
const char* ssid = STASSID;
const char* password = STAPSK;
const int Port = 9999;
char path [] = "/";
char host [] = "192.168.4.1";

//Laser & CDS setup
int cds = A0;
const int LaserPin = 13;
int LED = 9;
int count =0;
WebSocketClient webSocketClient;
WiFiClient client;

void setup() {
  // put your setup code here, to run once:  
  Serial.begin(115200);
  
  /**************************************/
  /***********Laser Sensor Set***********/
  
  pinMode(cds,INPUT); //CDS : 조도센서
  pinMode(LED, OUTPUT); //LED : 감지 ON/OFF 
  pinMode(LaserPin, OUTPUT); // Laser Module 
  pinMode(LED_BUILTIN, OUTPUT); //WIFI LED
  
  /*********Laser Sensor END************/
  /**************************************/

  /**************************************/
  /**********   WiFi Setup      *********/

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
    delay(1000);
  String data = "Sensor 1";  

  // put your main code here, to run repeatedly:
  digitalWrite(LaserPin, HIGH); 
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  cds = analogRead(A0);
  Serial.println(cds) ;
  if(cds < 700){digitalWrite(LED,HIGH);}
  else {digitalWrite(LED, LOW);}
  delay(500);
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  

  if ((client.connected())){
    //연결 완료
    //데이터 전송
    webSocketClient.sendData("msg count : "+String(count++));
    //Serial.print("Sensor 1\n");
    webSocketClient.getData(data);
    if(data.length() > 0)
      Serial.println(data);

    }else{
          Serial.print(".");
              return;
     }

    // capture the value of analog 1, send it along
    //pinMode(1, INPUT);
    //data = String(analogRead(1));

    
    //client.print("Sensor 1\n");
    //webSocketClient.sendData("[Arduino] Sensor 1 \n");
    

  }
