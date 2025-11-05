#include <Arduino.h>
#include "ARGBController.h"
#include "wificonfig.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html lang="es"><head> <meta name="viewport" content="width=device-width, initial-scale=1"> <title>Control de Luces</title> <link rel="icon" href="data:,"> <style> html {font-family: Arial; display: inline-block; text-align: center;}body { margin:0px auto; display: flex; background-color: #f4f7fa; }h1 { font-size: 28px; color: #3a4a63; /* Gris azulado oscuro */ margin-bottom: 25px; font-weight: bold;}h2 { font-size: 20px; color: #3a4a63; /* Gris azulado oscuro */ margin-bottom: 25px; font-weight: bold;}p { font-size: 3.0rem; margin: 15px;}label { font-size: 18px; color: #6c7c8b; /* Gris medio */ margin-bottom: 18px;}.button { display: inline-block; background-color: #5c6bc0; border: none; color: white; margin:10px; padding: 16px 40px; font-size: 30px; font-weight: bold; cursor: pointer; transition: all 0.3s ease; box-shadow: 0px 6px 8px #000000aa; border-radius: 10px;}.button:hover { background-color: #3f51b5; box-shadow: 0 6px 12px #00000099;}.button:active{ background-color: #3949ab; box-shadow: 0 6px 12px #00000000;}.switch {position: relative; display: inline-block; width: 120px; height: 68px} .switch input {display: none}.switcher {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}.switcher:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}input:checked+.switcher {background-color: #b30000}input:checked+.switcher:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}.container { background-color: #fff; border-radius: 16px; box-shadow: 0px 0px 20px #2c2e2c; margin: 20px; padding:20px;} </style></head><body> <div class="container"> <div class="row"> <h1>Control de Luces</h1> </div> <!-- control.rainbow(); break; case 1: control.rainbowStrips(strip,count); break; case 2: control.rainbowBlocks(strip,count); break; case 3: control.colour(); break;--> <div class="container"> <div class="row"> <h1>Controles de Ajuste</h1> </div> <h2 id="velocity"> Velocidad </h2> <input class="slider" type="range" onchange="updateSpeed(this)" id="SpeedSlider" min="0" max="100" value="15" step="1"> <h2 id="hue"> Tono </h2> <input class="slider" type="range" onchange="updateHue(this)" id="HueSlider" min="0" max="65535" value="15" step="1"> <h2 id="RGB"> RGB </h2> <input class="slider" type="range" onchange="updateRGB(this)" data-color="r" id="redSlider" min="0" max="255" value="15" step="1"> <input class="slider" type="range" onchange="updateRGB(this)" data-color="g" id="greenSlider" min="0" max="255" value="15" step="1"> <input class="slider" type="range" onchange="updateRGB(this)" data-color="b" id="blueSlider" min="0" max="255" value="15" step="1"> <br> <label class="switch"><input type="checkbox" onchange="toggleCheckbox(this)" id="modeSelectorCheck"><span class="switcher"></span></label> <br> <a class="button" id="colouraccepter" onclick="updateColour(this)" role="button" data-r="0" data-g="0" data-b="0" data-mode="0">Arcoiris</a> </div> <div class="container"> <div class="row"> <h1>Modos de Transición</h1> </div> <a class="button" onclick="changeTransitionMode(this)" role="button" data-mode="0">Arcoiris</a> <a class="button" onclick="changeTransitionMode(this)" role="button" data-mode="1">Strips</a> <a class="button" onclick="changeTransitionMode(this)" role="button" data-mode="2">Blocks</a> <a class="button" onclick="changeTransitionMode(this)" role="button" data-mode="3">Color solido</a> <div class="row"> <h1>Modos de Color</h1> </div> <a class="button" onclick="changeColourMode(this)" role="button" data-mode="0">RGB</a> <a class="button" onclick="changeColourMode(this)" role="button" data-mode="1">Hue</a> </div> </div> <script> function toggleCheckbox(element) { var acknowledge=document.getElementById("colouraccepter"); console.log(element.checked); acknowledge.dataset.mode=element.checked?1:0; console.log(acknowledge.dataset.mode); } function updateRGB(event){ var acknowledge=document.getElementById("colouraccepter"); switch(event.dataset.color){ case "r": acknowledge.dataset.r=event.value; break; case "g": acknowledge.dataset.g=event.value; break; case "b": acknowledge.dataset.b=event.value; break; default: console.log(`Caso Desconocido:${event.dataset.color}`); break; } acknowledge.style.setProperty('background-color',`rgba(${(acknowledge.dataset.r).toString(16)},${(acknowledge.dataset.g).toString(16)},${(acknowledge.dataset.b).toString(16)},1)`); console.log(event.dataset.color); console.log(event.value); } function updateColour(ev) { var xhr = new XMLHttpRequest(); var requester ="/settings?red=" + ev.dataset.r + "&green=" + ev.dataset.g + "&blue=" + ev.dataset.b + "&colour="+ev.dataset.mode+"&"; xhr.open("PUT",requester , true); console.log(requester); xhr.send(); } function updateHue(ev) { var xhr = new XMLHttpRequest(); var requester ="/settings?hue=" + ev.value; document.querySelector("#hue").innerHTML="Tono:"+ev.value; xhr.open("PUT",requester , true); console.log(requester); xhr.send(); } function updateSpeed(ev) { var xhr = new XMLHttpRequest(); var requester ="/settings?speed=" + ev.value + "&"; document.querySelector("#velocity").innerHTML="Velocidad:"+ev.value; xhr.open("PUT",requester , true); console.log(requester); xhr.send(); } function changeTransitionMode(element) { var xhr = new XMLHttpRequest(); var requester ="/mode?transition="+element.dataset.mode; xhr.open("PUT",requester , true); console.log(requester); xhr.send(); } function changeColourMode(element) { var xhr = new XMLHttpRequest(); var requester ="/mode?colour="+element.dataset.mode; xhr.open("PUT",requester , true); console.log(requester); xhr.send(); } document.getElementById("colouraccepter").style.setProperty('background-color',`rgba(${(document.getElementById("colouraccepter").dataset.r).toString(16)},${(document.getElementById("colouraccepter").dataset.g).toString(16)},${(document.getElementById("colouraccepter").dataset.b).toString(16)},1)`); </script></body></html>
)rawliteral";

#define WSPIN 13
#define LEDCOUNT 64
#define button 2

#define TRANSPARAMETER "transition"
#define COLOURPARAMETER "colour"
#define SPEEDPARAMETER "speed"
#define REDPARAMETER "red"
#define GREENPARAMETER "green"
#define BLUEPARAMETER "blue"
#define HUEPARAMETER "hue"

const char* ssid     = MySSID;
const char* password = MyPASSWORD;


int mode=0;
int strip=5;
int count[]={24,16,12,8,4};
ARGBController control=ARGBController(LEDCOUNT,WSPIN);

IPAddress local_IP(192, 168, 4, 2);
// We set a Gateway IP address
IPAddress gateway(192, 168, 4, 2);
IPAddress subnet(255, 255, 255, 0);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

unsigned long delayTime;
// Variable to store the HTTP request
String header;
// Define timeout time in milliseconds (example: 2000ms = 2s)
unsigned long lastTime = 0; 
unsigned long timerDelay = 100;


void Lights();

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
  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/mode", HTTP_PUT, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(TRANSPARAMETER)) {
      inputMessage1 = request->getParam(TRANSPARAMETER)->value();
      mode= inputMessage1.toInt();
    }
    else if (request->hasParam(COLOURPARAMETER)) {
        inputMessage1 = request->getParam(COLOURPARAMETER)->value();
        control.setMode(inputMessage1.toInt());
        Serial.print("Colour Mode Set as : ");
        Serial.println(inputMessage1);
    }else{
      inputMessage1 = "No message sent";
    }    
    request->send(200, "text/plain", "OK");
    Serial.print("Mode Set as : ");
    Serial.println(inputMessage1);
  });
  server.on("/settings", HTTP_PUT, [] (AsyncWebServerRequest *request) {
    String Speed, red, green, blue;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    
    if (request->hasParam(SPEEDPARAMETER)) {
        Speed = request->getParam(SPEEDPARAMETER)->value();
        control.setSpeed(Speed.toInt());
        Serial.print("Speed Set as : ");
        Serial.println(Speed);
    }else if (request->hasParam(HUEPARAMETER)) {
        Speed = request->getParam(HUEPARAMETER)->value();
        control.setHue(Speed.toInt());
        Serial.print("Speed Set as : ");
        Serial.println(Speed);
    }else if(request->hasParam(REDPARAMETER)&&request->hasParam(GREENPARAMETER)&&request->hasParam(BLUEPARAMETER)){
        Speed="0";
        red = request->getParam(REDPARAMETER)->value();
        green = request->getParam(GREENPARAMETER)->value();
        blue = request->getParam(BLUEPARAMETER)->value();
        if(request->hasParam(COLOURPARAMETER)){
            Speed = request->getParam(COLOURPARAMETER)->value();
        }
        if(Speed.toInt()==0){
            control.setInitColour(red.toInt(),green.toInt(),blue.toInt());
        }else{
            control.setFinalColour(red.toInt(),green.toInt(),blue.toInt());
        }
        Serial.print("Colour Start Set as : ");
        Serial.print(red);
        Serial.print(", ");
        Serial.print(green);
        Serial.print(", ");
        Serial.println(blue);    
    }
    request->send(200, "text/plain", "OK");
  });
  // Start server
  server.begin();
  Serial.print("Sending Events From Server:");
}
int ii=0;
void loop() {
  if ((millis() - lastTime) > timerDelay) {
    Lights();
    /*
    Serial.print("Index:");
    Serial.println(ii);
    Serial.print("Mapping:");
    Serial.println(control.customMap(ii,0,255));
    if(ii>0x10000){
      ii=0;
    }else{
      ii+=0x10000/255;
    }
    lastTime = millis();*/
  }

}

void Lights(){

    switch(mode%4){
        case 0:
        control.rainbow(strip,count);
        break;
        case 1:
        control.rainbowStrips(strip,count);
        break;
        case 2:
        control.rainbowBlocks(strip,count);
        break;
        case 3:
        control.colour(strip,count);
        break;
        default:
        break;
    }
}