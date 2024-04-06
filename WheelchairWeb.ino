#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DFRobot_DHT11.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include "homepage2.h"
#include "AccessGranted.h"
#include "AccessDenied.h"
#include "dataPage.h"
#include "controlsPage.h"
#include "Info.h"

#define DHT11_PIN 14
#define PN532_IRQ (18)
#define PN532_RESET (19)
const char* ssid = "Tamer";
const char* password = "tzraiq2005";

const int TRIG_PIN = 5;
const int ECHO_PIN = 17;
const int LED = 16;
const int MOTOR_PIN = 27;
const int ENABLE_PIN = 12;
const int ENABLE_PIN2 = 32;
const int MOTOR_PIN2 = 33;

const int FREQ = 30000;
const int PWM_CHANNEL = 0;
const int RESOLUTION = 8;
int dutyCycle = 0;
uint8_t success;
long duration;
float cms;

DFRobot_DHT11 DHT;
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

WebServer server(80);

bool checkAllowedUID(uint8_t uid[], uint8_t uidLength) {
  // Define the allowed UID
  uint8_t allowedUID[] = { 0x75, 0xFE, 0xF8, 0x3A };  // The nfc value is 75,FE,F8,3A
  // Compare the read UID with the allowed UID
  for (int i = 0; i < uidLength; i++) {
    if (uid[i] != allowedUID[i]) {
      return false;  // UID does not match allowed UID
    }
  }
  return true;  // UID matches allowed UID
}

String handleRFID() {
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;
  String granted, denied, undetected;
  granted = "Access Granted";
  denied = "Access Denied";
  undetected = "Card Undetected";
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
 
  if (success) {
    Serial.println("Card Detected");
    
    if (checkAllowedUID(uid, uidLength) == true) {//Check if the UID matches an allowed UID
      Serial.println(granted);
      return granted;
    } else{
      Serial.println(denied);
      return denied;
    }
  }else{
    return undetected;
    Serial.println(undetected);
  }
}

String getTemp() {
  DHT.read(DHT11_PIN);
  String temp = String(DHT.temperature);
  return temp;
}
String getFSR() {
  const int fsr = 34;
  int fsrRead;
  String occupied, unoccupied;
  //Pressure Sensor
  occupied = "Wheelchair Occupied";
  unoccupied = "Wheelchair Unoccupied";
  fsrRead = analogRead(fsr);
  delay(1000);
  if (fsrRead > 0) {
    return occupied;
  } else {
    return unoccupied;
  }
}
float reverseSensor(){
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  cms = (duration / 2) / 29.1;

  Serial.print(cms);
  Serial.println(" cm");
  
  if (cms < 5) {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }
}
void handleRoot() {
  String message;
  if(handleRFID() == "Access Granted"){
    message = homePagePart1; 
    server.send(200, "text/html", message);    
  }else{
    message = homePagePart0;
    server.send(200, "text/html", message);    
  }
}
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleKeyPress() {
  String receivedData = server.arg("button");
  if (receivedData == "L") {
    dutyCycle = 200;
    ledcWrite(PWM_CHANNEL, dutyCycle);
    digitalWrite(MOTOR_PIN, HIGH);
    digitalWrite(MOTOR_PIN2, HIGH);
  } else if (receivedData == "R") {
    dutyCycle = 200;
    ledcWrite(PWM_CHANNEL, dutyCycle);
    digitalWrite(MOTOR_PIN, LOW);
    digitalWrite(MOTOR_PIN2, LOW);
  } else if (receivedData == "F") {
    dutyCycle = 200;
    ledcWrite(PWM_CHANNEL, dutyCycle);
    digitalWrite(MOTOR_PIN, LOW);
    digitalWrite(MOTOR_PIN2, HIGH);
  } else if (receivedData == "B") {
    dutyCycle = 200;
    digitalWrite(MOTOR_PIN, HIGH);
    digitalWrite(MOTOR_PIN2, LOW);
    ledcWrite(PWM_CHANNEL, dutyCycle);
    reverseSensor();
  } else if (receivedData == "S") {
    ledcWrite(PWM_CHANNEL, 0);
  }

  Serial.println(receivedData);
  server.send(200);
}

void setup(void) {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  nfc.begin();
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

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/sentKeyPressToWebServer", handleKeyPress);

  server.on("/homepage2.html", []() {
    String message = homePagePart2;
    server.send(200, "text/html", message);
  });

  server.on("/info.html", []() {
    String message = information;
    server.send(200, "text/html", message);
  });

  server.on("/data.html", []() {
    String message = datapage + getTemp() + datapage1 + getFSR() + datapage3;
    server.send(200, "text/html", message);
  });

  server.on("/controls.html", []() {
    String message = controlspage;
    server.send(200, "text/html", message);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  ledcSetup(PWM_CHANNEL, FREQ, RESOLUTION);
  ledcAttachPin(ENABLE_PIN, PWM_CHANNEL);
  ledcAttachPin(ENABLE_PIN2, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, dutyCycle);
}

void loop(void) {
  server.handleClient();
  delay(2);  //allow the cpu to switch to other tasks
}
