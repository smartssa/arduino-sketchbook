/* 
*  Simple WiFi weather station with Arduino, the DHT11 sensor & the CC3000 chip
*  Part of the code is based on the work done by Adafruit on the CC3000 chip & the DHT11 sensor
*  Writtent by Marco Schwartz for Open Home Automation
*/

// Include required libraries
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "SocketIOClient.h"


// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// WiFi network (change with your settings !)
#define WLAN_SSID       "flatlinesystems.net"        // cannot be longer than 32 characters!
#define WLAN_PASS       "ilovebeer"
#define WLAN_SECURITY   WLAN_SEC_WPA2 // This can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2

// LED Status PINS sensor pins
#define OFFLINEPIN 7
#define ONLINEPIN 8

// Create CC3000 
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2);

// a socket client                                 
SocketIOClient client;
        
// Where to connect to the socket
// char hostname[] = "192.168.1.175";
uint32_t ip = cc3000.IP2U32(192,168,1,175);
char hostname[] = "jessica.local";
// char hostname[] = "echo.websocket.org";
int port = 9000;
                                         
void setup(void)
{
 
  pinMode(OFFLINEPIN, OUTPUT);
  pinMode(ONLINEPIN, OUTPUT);
  
  digitalWrite(OFFLINEPIN, HIGH);
  
  // TV needs 9600 baud.
  Serial.begin(9600);
    
  // Initialise the CC3000 module
  if (!cc3000.begin())
  {
    while(1);
  }

  // Connect to  WiFi network
  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  Serial.println("Connected to WiFi network!");
    
  // Check DHCP
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100);
  }  
  Serial.println("We Have Wifi");

  // connect to the socket
  client.setDataArrivedDelegate(ondata);
  if (!client.connect(cc3000, hostname, port, ip)) {
    Serial.println(F("Not connected."));
  }

}

void loop(void)
{

  // listen on the socket.  
  client.monitor(cc3000);
  if (client.connected()) { 
    client.sendEvent("something", "Client here!");
    digitalWrite(OFFLINEPIN, LOW);
    digitalWrite(ONLINEPIN, HIGH);
  } else {
    digitalWrite(ONLINEPIN, LOW);
    digitalWrite(OFFLINEPIN, HIGH);
  }    

}


// websocket message handler: do something with command from server
void ondata(SocketIOClient client, char *data) {
  // send the command over serial.
  Serial.println(data);
}


