#include <ThingSpeak.h> //ThingSpeak Library
#include <ESP8266WiFi.h> // WiFi Library
#include <MHZ19.h> //Library für den Sensor
#include <SoftwareSerial.h> //Library für die Kommunikation mit dem Sensor

#define RX_PIN D6 //Empfänger-Pin
#define TX_PIN D7 //Absender-Pin
#define BAUDRATE 9600

MHZ19 sensor; //Erstellen des Sensors
SoftwareSerial serial(RX_PIN, TX_PIN); //Erstellen der Verbindung
WiFiClient client; //Erstellen der WiFi Library

//WiFi Setup
const char* ssid = "WLAN_NAME"; //Name des drahtlosen Netzwerks
const char* password = "WLAN_PASSWORT"; //Passwort des Netzwerks

//ThingSpeak Setup
const char* writeAPIKey = "WRITE_API_KEY"; //Key um Daten auf ThingSpeak hochzuladen
const long int channelID = KANAL_ID; //ID des ThingSpeak Kanals



void setup() {
  serial.begin(BAUDRATE); //Verbindung mit dem Sensor erstellen
  sensor.begin(serial); //Sensor mit der Verbindung verknüpfen
  sensor.autoCalibration(); //Automatische Kalibrierung des Sensors aktivieren
  Serial.begin(9600); //Verbindung zum PC aufbauen
  connect_WiFi(); //Mit WiFi verbinden (eigene Funktion am Ende definiert)
}



void loop() {
  bool validValue = true; //Boolean für die Überprüfung des Messwertes
  int co2value = sensor.getCO2(); //co2-Wert vom Sensor auslesen
  int temperature = sensor.getTemperature(); //Temperatur vom Sensor auslesen
  Serial.println(String(co2value) + "ppm"); //co2-Werte im Serial-Monitor ausgeben
  Serial.println(String(temperature) + " C"); //Temperatur im Serial-Monitor ausgeben

  if (co2value == 0) { //Falls der Sensor den Wert 0 ausgibt, dann soll dieser nicht berücksichtigt werden
    validValue = false;
  }
  if (temperature == 0) { //Falls der Sensor den Wert 0 ausgibt, dann soll dieser nicht berücksichtigt werden
    validValue = false;
  }

  if (validValue) {
    ThingSpeak.setField( 1, co2value); //co2-Wert auf Field 1 eintragen
    ThingSpeak.setField( 2, temperature); //Temperatur Wert auf Field 2 eintragen
    ThingSpeak.writeFields(channelID, writeAPIKey); //Daten auf Thingspeak laden
  } else {
    Serial.println("last value ignored due to invalidity"); //Benutzer über die Umstände aufklären
  }
  delay(15000); //15 Sekunden warten, bevor erneut ein Wert erfasst und gesendet wird
}

void connect_WiFi() {
  WiFi.begin(ssid, password); //Verbindung aufbauen
  while (WiFi.status() != WL_CONNECTED) { //Warten, bis eine Verbindung aufgebaut ist
     delay(50);
     Serial.println("/");
     delay(50);
     Serial.println("_");
     delay(50);
     Serial.println("\\");
     delay(50);
     Serial.println("|");
  }

  Serial.println("Connected to WiFi"); //Bestätigung der Verbindung ausgeben
  ThingSpeak.begin(client); //ThingSpeak mit starten
  delay(500);
}
