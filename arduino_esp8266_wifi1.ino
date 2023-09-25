/*
Some References:

https://github.com/esp8266/Arduino/issues/7125
https://www.youtube.com/watch?v=2r8evY6-1M0&t=62s

This working code came from https://www.youtube.com/watch?v=RMQINbokCYg&t=104s


Ardunio to ESP8266 01S PIN connections

Arduino Pin2 to ESP8266 Tx

Arduino Pin3 to ESP8266 Rx

Arduino Gnd to ESP8266 Gnd

Arduino 3.3 v to ESP8266 3.3v + En 

Arduino IDE
File - > Preferences -> Additional Board Manager URLs http://arduino.esp8266.com/stable/package_esp8266com_index.json

in Tools - Manage Libraries search esp8266 and install



Then Tools - Board should be Arduino UNO
COM port selected

Serial Port display will be (time stamp enabled)


07:09:39.721 -> Connecting to Wifi
07:09:39.721 -> ..WIFI CONNECTED
07:09:48.179 -> IP Address = 


Ramki's Understanding:
This code is loaded in Arduino and not in ESP8266
This makes Arduino wifi enabled microcontroller through ESP8266

if you want to see IP address allocated, goto 192.168.0.1 home DLINK router and check the status, it will show some esp_ ip address


*/


#include <SoftwareSerial.h>
#include <stdlib.h>

SoftwareSerial ESP8266(2, 3); // RX, TX
unsigned char check_connection=0;
unsigned char times_check=0;
void setup() {
  Serial.begin(115200);
  ESP8266.begin(115200);  

   ESP8266.print("***VER:");
delay(2000);
ESP8266.println("AT+RST");
 delay(1000);
 ESP8266.println("AT+GMR");
delay(1000);
ESP8266.println("AT+CWMODE=3");
delay(1000);
ESP8266.println("AT+CWLAP");
delay(1000);
}


void loop() {
Serial.println("Connecting to Wifi");
   while(check_connection==0)
  {
    Serial.print(".");
  //ESP8266.print("AT+CWJAP=\"mobilehotspot name\",\"passwd\"\r\n");
  ESP8266.print("AT+CWJAP=\"2GHz home wifi name\",\"passwd\"\r\n");
  ESP8266.setTimeout(5000);
 if(ESP8266.find("WIFI CONNECTED\r\n")==1)
 {
 Serial.println("WIFI CONNECTED");
 //https://circuits4you.com/2019/01/05/connecting-esp8266-to-wifi-hotspot-example/
 Serial.println("IP Address = ");
 //Serial.println(“AT+CIFSR”);
 break;
 }
 times_check++;
 if(times_check>3) 
 {
  times_check=0;
   Serial.println("Trying to Reconnect..");
  }
  } 


   
  while(1);
}
