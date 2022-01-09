/*Arduino  code for ESP8266 AJAX Webserver

www.circuitdigest.com 
*/

// Parsing Stuffs
String Buffer, PH, Turbidity;
char c;
int Index1, Index2, Index3;

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "index.h"


//#define LED D0
const char* ssid = "Ayo Bisa";
const char* password = "12345678";
ESP8266WebServer server(80);



void handleRoot() 
{
 String s = webpage;
 server.send(200, "text/html", s);
}

void sensor_ph() 
{
  String data = PH;
  server.send(200, "text/plane", data);
  
}

void sensor_turbidity(){
  String data = Turbidity;
  server.send(200, "text/plane", data);
  }

//void led_control() 
//{
// String state = "OFF";
// String act_state = server.arg("state");
// if(act_state == "1")
// {
//  //digitalWrite(LED,HIGH); //LED ON
//  state = "ON";
// }
// else
// {
//  //digitalWrite(LED,LOW); //LED OFF
//  state = "OFF";
// }
// 
// server.send(200, "text/plane", state);
//}

void setup(void)
{
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  while(!Serial){}
//  Serial.println("");
  //pinMode(LED,OUTPUT); 
  
//  while (WiFi.status() != WL_CONNECTED)
//  {
//    Serial.print("Connecting...");
//  }
//  Serial.println("");
//  Serial.print("Connected to ");
//  Serial.println(ssid);
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());
 
  server.on("/", handleRoot);
  server.on("/phread", sensor_ph);
  server.on("/turbread", sensor_turbidity);
  server.begin();
}

void loop(void)
{
  parsing_data();
  server.handleClient();
}

void parsing_data(){
  while(Serial.available() > 0){
      delay(10);
      c = Serial.read();
      Buffer += c;
    }
  if(Buffer.length() > 0){
      Serial.println(Buffer);
      Index1 = Buffer.indexOf('|');
      Index2 = Buffer.indexOf('|', Index1+1);
      Index3 = Buffer.indexOf('|', Index2+1);
      PH = Buffer.substring(Index1+1, Index2);
      Turbidity = Buffer.substring(Index2+1, Index3);
      Buffer = "";
    }
  }
