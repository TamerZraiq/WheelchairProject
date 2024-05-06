#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DFRobot_DHT11.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <Adafruit_GPS.h>
#include "MAX30100_PulseOximeter.h"
#include "DHT_Async.h"

#include "homepage2.h"
#include "Emergency.h"
#include "AccessGranted.h"
#include "AccessDenied.h"
#include "dataPage.h"
#include "controlsPage.h"
#include "Info.h"
#include "secrets.h"
#include "ThingSpeak.h"

#define PN532_IRQ (18)
#define PN532_RESET (19) 
#define REPORTING_PERIOD_MS_THINGSPEAK 1000  // report to ThingSpeak every 1s
#define REPORTING_PERIOD_MS 1000
#define DHT_SENSOR_TYPE DHT_TYPE_11
#define GPSSerial Serial2
#define GPSECHO false

char ssid[] = SECRET_SSID;  // your network SSID (name)
char pass[] = SECRET_PASS;  // your network password
unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;
int keyIndex = 0;           // your network key Index number (needed only for WEP)

static const int DHT_SENSOR_PIN = 14;
const int TRIG_PIN = 5;
const int ECHO_PIN = 23;
const int LED = 13;
const int buzzer = 25;
const int MOTOR_PIN = 27;
const int ENABLE_PIN = 12;
const int ENABLE_PIN2 = 32;
const int MOTOR_PIN2 = 33;

// Initialize motor values
const int FREQ = 30000;
const int PWM_CHANNEL = 0;
const int RESOLUTION = 8;
int dutyCycle = 0;

uint32_t tsLastReportThingSpeak = 0;  //4 byte unsigned int to to time ThingSpeak 20s
uint32_t tsLastReport = 0;
int number = 0;
String myStatus = "";
float temperature;
float humidity;
float beat;

uint8_t success;
long duration;
float cms;

//for looping the main every 2 seconds
unsigned long previousMillis = 0;  //execute the loop in void main only if the current time is - the last time readings were taken is greater than 2 seconds
const long interval = 1000;        // Interval in milliseconds
uint32_t timer = millis();

DHT_Async dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
PulseOximeter pox;
Adafruit_GPS GPS(&GPSSerial);

WiFiClient client;
WebServer server(80);
int getTemp() {  
  //int temperature = DHT.temperature;
  return temperature;
}
String getHumidity() {
  //String humidity = String(DHT.humidity);
  return String(humidity);
}
String getHeart(){
  return String(beat);
}
// Callback fired when a pulse is detected
void onBeatDetected() {
  Serial.println("\nBeat!");
  Serial.print(beat);
  Serial.print("bpm / SpO2:");
  Serial.print(pox.getSpO2());
  Serial.println("%");
}
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
    Serial.println(undetected);
    return undetected;
    
  }
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
void reverseSensor(){
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
    digitalWrite(buzzer, HIGH);
    delay(250);
    digitalWrite(LED, LOW);
    digitalWrite(buzzer, LOW);
    delay(250);
  }
  delay(250);
}
 
//String to get the Google maps location to a 5 metre tolerance,
//on the webpage we are able to click a link that takes us to the location on googlemaps
String getGoogle() {
  float lati_v1;   // divide 100
  int lati_int;    //takes out the integer
  float lati_dec;  //takes out the decimal
  double lati_v2;  //final latitude
 
  lati_v1 = GPS.latitude / 100;
  lati_int = lati_v1;
  lati_dec = lati_v1 - lati_int;
  lati_dec = lati_dec / 0.6;
  lati_v2 = lati_int + lati_dec;
 
  float longi_v1;   // divide 100
  int longi_int;    //takes out the integer
  float longi_dec;  //takes out the decimal
  double longi_v2;  //final longitude
 
  longi_v1 = GPS.longitude / 100;
  longi_int = longi_v1;
  longi_dec = longi_v1 - longi_int;
  longi_dec = longi_dec / 0.6;
  longi_v2 = longi_int + longi_dec;
 
  //we are concatenating the latitude and longitude to the https, so that we can access the link on google
  String googleMapsLink = "https://www.google.com/maps?q=";
  googleMapsLink += String(lati_v2, 6);
  googleMapsLink += ",";
  googleMapsLink += String(-longi_v2, 6);
  return googleMapsLink;
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
  } else if (receivedData == "S") {
    ledcWrite(PWM_CHANNEL, 0);
  }

  Serial.println(receivedData);
  server.send(200);
}

void setup(void) {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  WiFi.begin(ssid, pass);
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
    String message = datapage + String(getTemp()) + datapage1 + String(getHeart()) + datapage2 + getHumidity() + datapage3 + getFSR() + datapage4;
    server.send(200, "text/html", message);
  });

  server.on("/controls.html", []() {
    String message = controlspage;
    server.send(200, "text/html", message);
  });
  server.on("/Emergency.html", []() {
    String message = emergency + getGoogle() + gpsData;
    server.send(200, "text/html", message);
  });
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  Serial.println("Initializing pulse oximeter..");

  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  ledcSetup(PWM_CHANNEL, FREQ, RESOLUTION);
  ledcAttachPin(ENABLE_PIN, PWM_CHANNEL);
  ledcAttachPin(ENABLE_PIN2, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, dutyCycle);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;)
      ;
  } else {
    Serial.println("SUCCESS");
  }
  pox.setOnBeatDetectedCallback(onBeatDetected);

  Serial.println("Adafruit GPS library basic parsing test!");
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);  // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);
}

static bool measure_environment(float* temperature, float* humidity) {
  static unsigned long measurement_timestamp = millis();
 
  /* Measure once every two seconds. */
  if (millis() - measurement_timestamp > 2000ul) {
    if (dht_sensor.measure(temperature, humidity)) {
      measurement_timestamp = millis();
      return (true);
    }
  }
 
  return (false);
}

void loop(void) {
  server.handleClient();
  pox.update();

  unsigned long currentMillis = millis();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
      //  Serial.print("Heart rate:");
      beat = pox.getHeartRate();
      //  Serial.print(beat);
      //  Serial.print("bpm / SpO2:");
      //  Serial.print(pox.getSpO2());
      //  Serial.println("%");

      tsLastReport = millis();
  }
  if (millis() - timer > 1000) {
    reverseSensor(); 
  }
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    // Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA()))  // this also sets the newNMEAreceived() flag to false
      return;                        // we can fail to parse a sentence in which case we should just wait for another
  }
  if (millis() - timer > 2000) {
    timer = millis();  // reset the timer
    Serial.print("\nTime: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC);
    Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC);
    Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC);
    Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC);
    Serial.print('/');
    Serial.print(GPS.month, DEC);
    Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: ");
    Serial.print((int)GPS.fix);
    Serial.print(" quality: ");
    Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude / 100, 2);
      Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(-GPS.longitude / 100, 2);
      Serial.println(GPS.lon);
      //Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      //Serial.print("Angle: "); Serial.println(GPS.angle);
      // Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: ");
      Serial.println((int)GPS.satellites);
    }
  }
    /* Measure temperature and humidity.  If the functions returns
        true, then a measurement is available.
        the parameters of measure_environment are pointers of the variables where the readings are stored */
  if (measure_environment(&temperature, &humidity)) {
    Serial.print("\nTemp:");
    Serial.print(temperature, 1);
    Serial.print("  Humi:");
    Serial.print(humidity, 1);
    Serial.println("%");
  }
  /* To set the field values with the collected data, the setField () is used to assign 
  the temperature and humidity  values to specific fields in the ThingSpeak channel*/
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, beat);
  updateTS();
}

void updateTS() {
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
 
    // pieces of information in a channel.  Here, we write to field all the fields.
    //To send the data to ThingSpeak, the function writeFields () is used
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); 
    if (!pox.begin()) {
      Serial.println("FAILED");
      for (;;);
    } else {
      Serial.println("SUCCESS");
    }
 
    pox.setOnBeatDetectedCallback(onBeatDetected);
    pox.update();
 
    tsLastReport = millis();
 
  }
}