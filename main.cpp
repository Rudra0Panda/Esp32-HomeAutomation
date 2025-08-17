#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#define RELAY1 D1   
#define RELAY2 D2   
#define SW1    D5   
#define SW2    D6   
const char* ssid = "Galaxy F23 5G 73C2";
const char* password = "zxre9606";
bool relay1State = false;  
bool relay2State = false;  
bool lastSW1 = HIGH;  
bool lastSW2 = HIGH;
AsyncWebServer server(80);
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP8266 Relay Control</title>
  <style>
    body { font-family: Arial; text-align: center; }
    .btn { padding: 15px 30px; margin: 10px; font-size: 20px; cursor: pointer; }
    .on { background-color: green; color: white; }
    .off { background-color: red; color: white; }
  </style>
  
</head>
<body>
  <h2>ESP8266 Relay Control</h2>
  <p>Relay 1</p>
  <button class="btn on" onclick="fetch('/relay1/on')">ON</button>
  <button class="btn off" onclick="fetch('/relay1/off')">OFF</button>
  <p>Relay 2</p>
  <button class="btn on" onclick="fetch('/relay2/on')">ON</button>
  <button class="btn off" onclick="fetch('/relay2/off')">OFF</button>
</body>
</html>
)rawliteral";
void setup() {
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  server.on("/relay1/on", HTTP_GET, [](AsyncWebServerRequest *request){
    relay1State = true;
    digitalWrite(RELAY1, LOW);
  });
  server.on("/relay1/off", HTTP_GET, [](AsyncWebServerRequest *request){
    relay1State = false;
    digitalWrite(RELAY1, HIGH);
  });
  server.on("/relay2/on", HTTP_GET, [](AsyncWebServerRequest *request){
    relay2State = true;
    digitalWrite(RELAY2, LOW);
  });
  server.on("/relay2/off", HTTP_GET, [](AsyncWebServerRequest *request){
    relay2State = false;
    digitalWrite(RELAY2, HIGH);
  });
  server.begin();
}
void loop() {
  bool currentSW1 = digitalRead(SW1);
  bool currentSW2 = digitalRead(SW2);
  if (currentSW1 != lastSW1) {
    delay(20); // debounce
    if (digitalRead(SW1) == currentSW1) {
      relay1State = !relay1State;
      digitalWrite(RELAY1, relay1State ? LOW : HIGH);
      lastSW1 = currentSW1;
    }
  }
  if (currentSW2 != lastSW2) {
    delay(20); 
    if (digitalRead(SW2) == currentSW2) {
      relay2State = !relay2State;
      digitalWrite(RELAY2, relay2State ? LOW : HIGH);
      lastSW2 = currentSW2;
    }
  }
}
