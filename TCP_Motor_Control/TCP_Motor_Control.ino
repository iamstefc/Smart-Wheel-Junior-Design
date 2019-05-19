// Code developed and compiled by Stefano & Irina
#include <SoftwareSerial.h>
#include "DRV8833.h"
#include "ESP8266.h" // Library found: http://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html
#include "string.h"
#define TIMEOUT 5000 //ms

// initializations for motors to be controlled
const int inA1 = 5; // pin 5 on teensy lc
const int inB1 = 9; // pin 9 on teensy lc

// initialization for DRV8833 & ESP8266 components
// for controlling two motors & connecting devices
DRV8833 driver = DRV8833();
ESP8266 wifi(Serial1);
SoftwareSerial mySerial(0,1); // 0 for RX; 1 for TX

// prototype functions to be used
boolean echoFind(String keyword);
boolean SendCommand(String cmd, String ack);

void setup() {
   // Open serial communication and wait for port to open:
   Serial.begin(115200);
   mySerial.begin(115200); //defaulted to pins 0 and 1 on teensy lc, RX/TX
   
   // AT commands used to identify & permit communication between devices
   // Found: http://room-15.github.io/blog/2015/03/26/esp8266-at-command-reference/#AT+CWJAP 
   SendCommand("AT+RST", "Ready"); // restarts module to begin new module
   delay(5000); // delay allows "OK" response to appear, too fast means unable to connect
   SendCommand("AT+CWMODE=3","OK"); // where, 1 = station; 2 = access point; 3 = both
   // AT+CWJAP=SSID,PWD allows ESP8266 to connect to access point w/ given credentials
   SendCommand("AT+CWJAP=\"iot_lab\",\"44FEC4DDB9\"", "OK"); // connect to access point
   SendCommand("AT+CIFSR", "OK"); // attain local IP address
   SendCommand("AT+CIPMUX=1","OK"); // where, 0 = single connection; 1 = multiple connections
   // configure communication with this side being SERVER   
   // makes ESP8266 the server of interaction & assigns port number as 8865
   SendCommand("AT+CIPSERVER=1,8865","OK"); // where, 0 = delete server; 1 = create server
   
   Serial.println("Server is created");
   
   // Motors assigned from input pins:
   pinMode(inA1, OUTPUT);
   pinMode(inB1, OUTPUT);
   Serial.println("Ready!");
}

void loop() {
   uint8_t buffer[128] = {0};
   char *str;
   char data[128]= {0};
   String message;
   uint8_t mux_id;
   int t=0;
   
   // 
   uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);
   if (len > 0){
      Serial.print("Status:[");
      Serial.print(wifi.getIPStatus().c_str()); // get IP
      Serial.println("]");
      
      Serial.print("Received from :");
      Serial.print(mux_id); // id identified in communication
      Serial.print("[");
	  
	  // print contents from the buffer
      for(uint32_t i = 0; i < len; i++) {
          Serial.print((char)buffer[i]);
          data[t] = (char)buffer[i];
          t++;
      }
      Serial.print("]\r\n"); // ends the print
	  // message could be "ford" or "stop" controlling the wheels
      message = String(data);
    }
	
	// moves the wheels forward
    if(message == "ford"){
      digitalWrite(inA1, HIGH);
      digitalWrite(inB1, HIGH);
      delay(1000); // immediate
    }
	
	// stops the wheels
    if(message == "stop"){
      digitalWrite(inA1, LOW);
      digitalWrite(inB1, LOW);
      delay(1000);
    }
}

// Flag function prints user input command
boolean SendCommand(String cmd, String ack){
  Serial1.println(cmd);
  if(!echoFind(ack))
      return true;
}

// Flag function identifies user input keyword, if matches
boolean echoFind(String keyword){
 byte current_char = 0;
 byte kk = keyword.length();
 long deadline = millis() + TIMEOUT;
 while(millis() < deadline){
  // transmit available data to determine movement of motors
  if (mySerial.available()){
    char ch = mySerial.read(); // reads content
    Serial.write(ch);		   // write, read content
    if (ch == keyword[current_char])
      if (++current_char == keyword_length){
       Serial.println(); 
       return true;
    }
   }
  }
 return false; // Timed out
}


