#include <Arduino.h>
#include "wificonfig.h"
#include "htmlconfig.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>


const char* ssid     = MySSID;
const char* password = MyPASSWORD;

IPAddress local_IP(192, 168, 2, 2);
// We set a Gateway IP address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

unsigned long delayTime;
// Variable to store the HTTP request
String header;
// Define timeout time in milliseconds (example: 2000ms = 2s)
unsigned long lastTime = 0; 
unsigned long timerDelay = 2000;

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200); 
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid);
  WiFi.softAPConfig (local_IP, gateway, subnet);
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", index_html);
  });
  // Start server
  server.begin();
  Serial.print("Sending Events From Server:");
}
void loop() {

}