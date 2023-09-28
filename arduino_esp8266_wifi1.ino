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

//AANOTHER PROJECT
//********************************************************************************************

/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  


  Ramki's Notes:

Working Project
https://randomnerdtutorials.com/esp8266-web-server/


  Plugin ESP8266 to ESP-01 Programmer (came from Amazon)
REES52
REES52 2PCS USB to ESP8266 Adapter Module ESP-01 Prog ESP-01S Programmer Downloader CH340C Driver with Reset Button, Easy to useREES52 2PCS USB to ESP8266 Adapter Module ESP-01 Prog ESP-01S Programmer Downloader CH340C Driver with Reset Button, Easy to use


Now connect ESP-01 programmer to Laptop USB port

In Ardunio 2.0 IDE in preferences add http://arduino.esp8266.com/stable/package_esp8266com_index.json in Additional Board Managers URL
Arudunio IDE Tools - Manager Libraries  search esp8266 and install

in Tools Board select ESP8266 Generic ESP8266 Module

COM port

Baud rate in Serial is 1152000

In ESp-01 programmer press reset button
now upload this sketch and it will get uploaded to ESP8266

ESP8266 is assigned an IP address from your home router 

if you goto Chrome and give the IP address 192.168.0.117 you should see on off buttons

connect Gnd to Gnd
01 and 02 IO pins to LED +ve pin

through 330 OHMs resistor LED -ve pin connected to Gnd

if you press ON ib browser LED shd get on and vice versa


*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "dlink-2286";
const char* password = "password";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output2State = "off";
String output0State = "off";

// Assign output variables to GPIO pins
const int output2 = 2;
const int output0 = 0;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output2, OUTPUT);
  pinMode(output0, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output2, LOW);
  digitalWrite(output0, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output2State = "on";
              digitalWrite(output2, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output2State = "off";
              digitalWrite(output2, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output0State = "on";
              digitalWrite(output0, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output0State = "off";
              digitalWrite(output0, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>GPIO 5 - State " + output2State + "</p>");
            // If the output2State is off, it displays the ON button       
            if (output2State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 4  
            client.println("<p>GPIO 4 - State " + output0State + "</p>");
            // If the output0State is off, it displays the ON button       
            if (output0State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
