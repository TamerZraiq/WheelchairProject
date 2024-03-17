#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DFRobot_DHT11.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include "homepage3.h"
#include "homepage1.h"
#include "dataPage.h"
#include "controlsPage.h"

#define DHT11_PIN 14
#define PN532_IRQ (18)
#define PN532_RESET (19)
const char* ssid = "Tamer";
const char* password = "tzraiq2005";

const int MOTOR_PIN = 27;
const int ENABLE_PIN = 12;
const int ENABLE_PIN2 = 32;
const int MOTOR_PIN2 = 33;
int dutyCycle = 0;
const int FREQ = 30000;
const int PWM_CHANNEL = 0;
const int RESOLUTION = 8;
uint8_t success;
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
  return denied;
  if (success) {
    Serial.println("Card Detected");
    // Example: Check if the UID matches an allowed UID
    if (checkAllowedUID(uid, uidLength) == true) {
      Serial.println(granted);
      return granted;
      // Perform actions for granted access
    } else{
      Serial.println(denied);
      return denied;
      // Perform actions for denied access
    }
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
void handleRoot() {
  String message = homePagePart0 + handleRFID() + AccessStatus;
  server.send(200, "text/html", message);
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
  } else if (receivedData == "S") {
    ledcWrite(PWM_CHANNEL, 0);
  }

  Serial.println(receivedData);
  server.send(200);
}

void setup(void) {
  Serial.begin(115200);
  nfc.begin();
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

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/sentKeyPressToWebServer", handleKeyPress);

  server.on("/homepage3.html", []() {
    String message = homePagePart1;
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
