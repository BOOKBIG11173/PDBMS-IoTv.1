#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266HTTPClient.h>
#include <MicroGear.h>
#include "DHT.h"
/*------------------------------*/
#define relay D5
#define DHTPIN D3
#define DHTTYPE DHT22
#define APPID   "temphumi"
#define KEY     "rmVT7lZecS5roGX"
#define SECRET  "WzN0ZCu4DAsLb2mnrzz5KUvua"
#define ALIAS   "web"
//#define SECONDS_DS(seconds) ((seconds)*1000000UL)
#define WIFI_SSID "0918787331"
#define WIFI_PASSWORD "0918787331"
#define web_server "http://104.199.141.40/"    //IP web server
//#define figger_p  "DA F3 84 CC D9 9A 1C 22 79 EF C2 B5 88 7F 8A 49 F5 56 0B 3D" // Figger print grc.com/fingerprints.htm

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
WiFiClient client;
MicroGear microgear(client);

void setup() {
lcd.begin();
  Serial.begin(9600);                         //Serial connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);        //WiFi connection
  while (WiFi.status() != WL_CONNECTED) {     //Wait for the WiFI connection completion
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP:"+WiFi.localIP());
  dht.begin();
  pinMode(relay,OUTPUT);
  digitalWrite(relay,LOW);
  /* Initial with KEY, SECRET and also set the ALIAS here */
    microgear.init(KEY,SECRET,ALIAS);

    /* connect to NETPIE to a specific APPID */
    microgear.connect(APPID);
  microgear.on(CONNECTED,onConnected);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {        //Check WiFi connection status
    float tem =  dht.readTemperature();
    float hu = dht.readHumidity();
    int id=1;
    HTTPClient http;
    http.begin(web_server);
    //http.begin(web_server,figger_p);          // Figger print Begin
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.POST("id="+String(id)+","+String(tem)+","+String(hu));    //Sent Data to Database
    String res = http.getString();
    Serial.println(res);
    http.end();
    if (microgear.connected()) {
      microgear.chat(ALIAS,String(tem)+","+String(hu));
      Serial.println("send to mg = "+String(tem)+","+String(hu));
    }else{
      microgear.connect(APPID);
    }
    Serial.print("----------------------------\n");
  lcd.setCursor(0, 0);
  lcd.print("Temp  : ");
  lcd.print((tem), 2);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Humid : ");
  lcd.print((hu), 2);
  lcd.print(" %");
   Serial.print("Humidity: ");    // show in serial monitor
   Serial.print(hu);
   Serial.print(" %\n");
   Serial.print("Temperature: ");    // show in serial monitor
   Serial.print(tem);
   Serial.print(" *C \n");
   if(hu >= 80){
   digitalWrite(relay,HIGH);
    Serial.print("Fan ON\n");}
   else{
   digitalWrite(relay,LOW);
   Serial.print("Fan OFF\n");}
  }
  Serial.print("----------------------------\n");
  delay(10000);                      //Send request every 20 seconds
  //Serial.println("Sleeping Mode ...");
  //ESP.deepSleep(SECONDS_DS(20));     //Sleep mode every 10 seconds
  Serial.print("----------------------------\n");
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    microgear.setName(ALIAS);
}


