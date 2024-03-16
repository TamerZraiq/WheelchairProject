//header files handling wifi, webpage, sensors and servo
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "homepage.h"
#include <DFRobot_DHT11.h>
#include <DFRobot_Heartrate.h>


#define DHT11_PIN 14
//creating instances of the classes

DFRobot_DHT11 DHT;
DFRobot_Heartrate heartrate(DIGITAL_MODE);
//wifi credentials
const char* ssid = "Tamer";
const char* password = "tzraiq2005";
//defining pin numbers



const int TRIG_PIN = 5;
const int ECHO_PIN = 18;
const int LED = 19;
const int buzzer = 17;
const int fsr = 34;
const int heartSensor = 25;
int fsrRead;
long duration;
float cms;
String heartRateGl;
//web server on port 80
WebServer server(80);
//temp function to simulate temp sensor
String getTemp() {
  DHT.read(DHT11_PIN);
  String temp = String(DHT.temperature);
  return temp;
}
String getHeartRate() {
  int rateValue;
  heartrate.getValue(heartSensor);
  rateValue = heartrate.getRate();

  if (rateValue > 0) {
    Serial.println(rateValue);
    heartRateGl = String(rateValue);
    return heartRateGl;
  }
}

String getFSR() {
  String occupied, unoccupied;
  //Pressure Sensor
  occupied = "Wheelchair Occupied";
  unoccupied = "Wheelchair Unoccupied";
  fsrRead = analogRead(fsr);
  delay(1000);
  if (fsrRead > 0) {
    Serial.println(occupied);
    return occupied;
  } else {
    Serial.println(unoccupied);
    return unoccupied;
  }
}
//generates the webpage with the content
void handleRoot() {
  String message1 = homePagePart0 + getFSR() + homePagePart1 + getTemp() + homePagePart2 + getHeartRate() + homePagePart3;
  server.send(200, "text/html", message1);
}
//handles requests for non-existent pages
void handleNotFound() {
  String message1 = "File Not Found\n\n";
  message1 += "URI: ";
  message1 += server.uri();
  message1 += "\nMethod: ";
  message1 += (server.method() == HTTP_GET) ? "GET" : "POST";
  message1 += "\nArguments: ";
  message1 += server.args();
  message1 += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message1 += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message1);
}

void setup() {
  Serial.begin(115200);//initialize serial comms
  
  //connects to wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //sets up MDNS
    if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  //defines server routes  
  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  //initialize pin modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  
  server.handleClient();//handle web server requests
  delay(2);  //allow the cpu to switch to other tasks
  
  //measuring distance
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  cms = (duration / 2) / 29.1;

  Serial.print(cms);
  Serial.print(" cm");
  Serial.println();

  if (cms < 5) {
    digitalWrite(LED, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    digitalWrite(buzzer, LOW);
    delay(500);
  }
  delay(500);
}
