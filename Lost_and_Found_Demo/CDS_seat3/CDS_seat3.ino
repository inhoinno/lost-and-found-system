  
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketClient.h>

#ifndef STASSID
#define STASSID "raspi_SSongJeong"
#define STAPSK  "32152332"
#define LIMIT 10
#endif
String DEVICE_NAME = "CDS 0 : ";

int s_status = WL_IDLE_STATUS;
const char* ssid = STASSID;
const char* password = STAPSK;
const int Port = 9999;
char path [] = "/";
char host [] = "192.168.4.1";
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

void setup()
{
  Serial.begin(115200);       
  Serial.println("CDS Sensor Setup Start....");
  /**************************************/
  /***********CDS Sensor Set***********/                 
  pinMode(A0, INPUT);
  pinMode(D8, OUTPUT);
    /*********CDS Sensor END************/
  /**************************************/
  Serial.println("CDS Sensor Setup Done.");
  
  Serial.println("WiFI Setup Start...");
  _WiFi_setup();
    Serial.println("WiFi Setup Done.");
}

void loop()
{
    String msg = "Lost Detected";  

    int cds = analogRead(A0);
    Serial.println(cds);
    delay(100);
    
  if(cds<750)
  {
    //We detected the Lost
    Serial.print("Lost Detected !");
    digitalWrite(D8, HIGH);
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
    {digitalWrite(D8,LOW);}
  
}
