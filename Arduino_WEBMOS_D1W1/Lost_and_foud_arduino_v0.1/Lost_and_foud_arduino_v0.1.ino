#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#ifndef STASSID
#define STASSID "raspi_SSongJeong"
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
int cds1 = A0;
int cds2 = A1;

int count =0;

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
  Serial.println("Laser Setup Start...");
  /**************************************/
  /***********Laser Sensor Set***********/
  
  pinMode(cds1,INPUT); //CDS : 조도센서
  pinMode(cds2,INPUT); //
  pinMode(13, OUTPUT); //LED : 감지 ON/OFF 

  /*********Laser Sensor END************/
  /**************************************/
Serial.println("Laser Setup Done.");
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
   delay(1000);
  String data = "Sensor 1";  

     // Turn the LED on (Note that LOW is the voltage level
  cds1 = analogRead(A0);
  cds2 = analogRead(A1);
  
  Serial.println("cds1 : ", cds1);
  Serial.println("cds2 : ", cds2);

  /*
  if(cds < 800){
    digitalWrite(sensor,HIGH);
    digitalWrite(LED_BUILTIN, HIGH);}
  else {digitalWrite(sensor, LOW);
  digitalWrite(LED_BUILTIN, LOW);}
  */

  if(cds1 < 800 or cds2 < 800){
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)

  if ((client.connected())){
    //연결 완료
    //데이터 전송
    webSocketClient.sendData("msg count : "+String(count++));
    Serial.println(String(count)+"Send.");
    //webSocketClient.getData(data);
    //if(data.length() > 0)
      //Serial.println(data);

    }else{
       Serial.print(".");
       _WiFi_setup();  //A Line to avoid webSocket Error 1006 : "connection failed with no reason"
      //When Connection is Failed Due to websocket Network error, Try WiFi.begin again 
              return;
     }

    //webSocketClient.sendData("msg count : "+String(count++));
    // capture the value of analog 1, send it along
    //pinMode(1, INPUT);
    //data = String(analogRead(1));

    
    //client.print("Sensor 1\n");
    //webSocketClient.sendData("[Arduino] Sensor 1 \n");
    

  }
