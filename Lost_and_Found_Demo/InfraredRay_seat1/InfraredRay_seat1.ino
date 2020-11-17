#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WebSocketClient.h>

#ifndef STASSID
#define STASSID "raspi_SSongJeong"
#define STAPSK  "32152332"
#define LIMIT 10
#endif

//WiFi setup
String DEVICE_NAME = "Infrared 0 : ";
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
void setup() {
  // put your setup code here, to run once:

    
  Serial.begin(115200);
  Serial.println("Infrared Sensor Start...");
  /**************************************/
  /***********Infrared-Ray Sensor Set***********/
  
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  //pinMode(D4, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  /*********Infrared-Ray Sensor END************/
  /**************************************/
Serial.println("Pressure  Setup Done.");
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
  while (WiFi.status() != WL_CONNECTED or count > LIMIT) {
    delay(500);
    Serial.print(".");
    count++;
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

 // 적외선 감지 센서 부터 센서값을 읽습니다.
 // 감지되면 0, 감지되지 않으면 1이 나옵니다.

  int s1 = digitalRead(D2);
  int s2 = digitalRead(D3);
 // int s3 = digitalRead(D4);

  Serial.print("s1 = ");
  Serial.println(s1);
  Serial.print("s2 = ");
  Serial.println(s2);
  //Serial.print("s3 = ");
  //Serial.println(s3);

  
// 측정된 센서값이 0(감지)면 아래 블록을 실행합니다.
  if(s1 == 0 or s2 == 0 ){
    Serial.println(" [Infrared] Lost Detected! ");
    digitalWrite(LED_BUILTIN, LOW);
  }
    digitalWrite(LED_BUILTIN, HIGH);

  if ((client.connected())){
    //연결 완료
    //데이터 전송
      if(s1 ==  0 or s2 == 0 ){
        webSocketClient.sendData(DEVICE_NAME +msg);    
        Serial.println("Detected, Send.");
      }else{
        webSocketClient.sendData("I.");    
      }
    }else{
       Serial.print(".");
       _WiFi_setup();  //A Line to avoid webSocket Error 1006 : "connection failed with no reason"
      //When Connection is Failed Due to websocket Network error, Try WiFi.begin again 
              return;
     }    

  }
