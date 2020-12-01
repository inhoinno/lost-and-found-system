  
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketClient.h>

#ifndef STASSID
#define STASSID "raspi_SSongJeong"
#define STAPSK  "32152332"
#define LIMIT 10
#endif
String DEVICE_NAME = "Ultra-Sonic 0 : ";

int s_status = WL_IDLE_STATUS;
const char* ssid = STASSID;
const char* password = STAPSK;
const int Port = 9999;
char path [] = "/";
char host [] = "192.168.4.1";
int count =0;
WebSocketClient webSocketClient;
WiFiClient client;

int trig0 = D2;                                    
int echo0 = D3;                                 
int trig1 = D10;
int echo1 = D11;
int greed = D4;

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

void setup()
{
  Serial.begin(115200);       
  Serial.println("Ultra-Sonic Sensor Setup Start....");
  /**************************************/
  /***********Ultra-Sonic Sensor Set***********/                 
  pinMode(trig0, OUTPUT);                   
  pinMode(echo0, INPUT);                   
  pinMode(trig1, OUTPUT);                 
  pinMode(echo1, INPUT);
  pinMode(greed, OUTPUT); 
    /*********Ultra-Sonic Sensor END************/
  /**************************************/
  Serial.println("Ultra-Sonic Sensor Setup Done.");
  
  Serial.println("WiFI Setup Start...");
  _WiFi_setup();
    Serial.println("WiFi Setup Done.");
}

void loop()
{
    String msg = "Lost Detected";  

  digitalWrite(trig0, LOW);  // tirg 핀에 LOW신호를
  delayMicroseconds(10);      //딜레이 (작동중지 시간)
  digitalWrite(trig0, HIGH); // tirg 핀에 HIGH신호를.
  delayMicroseconds(10);      //딜레이 (작동중지 시간)
  digitalWrite(trig0, LOW);  // tirg 핀에 LOW신호를
  int distance1 = pulseIn(echo0, HIGH) * 17 / 1000;    //아래에 따로 설명. 
  
  delay(50);   
  
  digitalWrite(trig1, LOW);  // tirg 핀에 LOW신호를
  delayMicroseconds(10);      //딜레이 (작동중지 시간)
  digitalWrite(trig1, HIGH); // tirg 핀에 HIGH신호를.
  delayMicroseconds(10);      //딜레이 (작동중지 시간)
  digitalWrite(trig1, LOW);  // tirg 핀에 LOW신호를       
  int distance2 = pulseIn(echo1, HIGH) * 17 / 1000;

  
  Serial.print(distance1);                            // 시리얼 모니터에 distance를 표시함
  Serial.println("cm");                              //cm를 표시하고 줄바꿈.
  Serial.print(distance2);                            // 시리얼 모니터에 distance를 표시함
  Serial.println("cm");
  
  delay(50);                                        //딜레이 (작동중지 시간)
  if((distance1 > 0 && distance1 < 13 )||(distance2 >0 && distance2 < 13))
  {
    //We detected the Lost
    Serial.print("Lost Detected ! : [d1 "+String(distance1)+":d2 "+String(distance2)+"]");
    digitalWrite(greed, LOW);
    digitalWrite(greed, HIGH);
    if((client.connected())){
       webSocketClient.sendData(DEVICE_NAME +msg);    
        Serial.println("Detected, Send.");
      }else{Serial.print(".");
       _WiFi_setup();  //A Line to avoid webSocket Error 1006 : "connection failed with no reason"
      //When Connection is Failed Due to websocket Network error, Try WiFi.begin again 
              //return;
        }
    }
    else
    {digitalWrite(greed,LOW);}
  
}
