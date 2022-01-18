#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

const char* ssid = "MINISTRY OF TRUTH";
const char* password = "minitrue";
const String nodeID = "aquilla_psu";
bool relayStatus;

int ledPin = 2; 

ESP8266WebServer server(80);

const byte cmdOFF[] = {0xA0, 0x01, 0x00, 0xA1};
const byte cmdON[] = {0xA0, 0x01, 0x01, 0xA2};

void setup() {
  pinMode(0, OUTPUT);
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  WiFi.hostname(nodeID);
  


  server.on("/", sendIndex);
  server.on("/open", openRelay);
  server.on("/close", closeRelay);
  server.on("/status", getRelayStatus);
  server.on("/toggle", toggleRelay);
  server.onNotFound(sendNotFound);
  
  server.begin();
}

void loop() {
  server.handleClient();
}

void sendIndex() {
  server.send(200, "text/html", "<head><meta name=viewport"
    "content='width=device-width,initial-scale=1'></head>"
    "<p>To open relay, click this link:</p>"
    "<a href='/open' target='_blank'>Open Relay</a>"
    "<p>To close relay, click this link:</p>"
    "<a href='/close' target='_blank'>Close Relay</a>");
}
void openRelay() {
  Serial.write(cmdON, 4);
  server.send(200, "text/plain", "on");
  relayStatus = true;
}

void closeRelay() {
  Serial.write(cmdOFF,4);
  server.send(200, "text/plain", "off");
  relayStatus = false;
}

void getRelayStatus() {
  server.send(200, "text/plain", relayStatus ? "on" : "off");
}

void toggleRelay() {
  Serial.write(relayStatus ? cmdOFF : cmdON, 4);
  server.send(200, "text/plain", relayStatus ? "off" : "on");
  relayStatus = !relayStatus;
}

void sendNotFound() {
  server.send(404, "text/plain", "404: Not Found");
}
