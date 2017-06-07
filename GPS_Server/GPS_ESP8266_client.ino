/*
 * TCP/IP communication ESP8266WiFi
 * Copyright (C) 2017 http://www.projetsdiy.fr - https://www.diyprojects.io
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <TinyGPS++.h>                                  // Tiny GPS Plus Library
#include <SoftwareSerial.h>     

#include <ESP8266WiFi.h>
const char* ssid     = "WILLIAM CRUZ";      // SSID
const char* password = "A1B2C3D4";      // Password
const char* host = "192.168.0.28";  // IP serveur - Server IP
const int   port = 8080;            // Port serveur - Server Port
const int   watchdog = 5000;        // Fréquence du watchdog - Watchdog frequency
unsigned long previousMillis = millis(); 

static const int RXPin = 12, TXPin = 13;                // Ublox 6m GPS module to pins 12 and 13
static const uint32_t GPSBaud = 9600;                   // Ublox GPS default Baud Rate is 9600
//
const double Home_LAT = 4.632060;                      // Your Home Latitude
const double Home_LNG = -74.082119;                     // Your Home Longitude

double lati = 0;
double longi = 0;

TinyGPSPlus gps;                                        // Create an Instance of the TinyGPS++ object called gps
SoftwareSerial ss(RXPin, TXPin);                        // The serial connection to the GPS device

void setup() {

  ss.begin(GPSBaud);                                    // Set Software Serial Comm Speed to 9600
  
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void loop() {

  smartDelay(500);
  
  unsigned long currentMillis = millis();
 
  if ( currentMillis - previousMillis > watchdog ) {
    previousMillis = currentMillis;
    WiFiClient client;
  
    if (!client.connect(host, port)) {
      Serial.println("connection failed");
      return;
    }

   lati = gps.location.lat();
   longi = gps.location.lng();

  

//   char latt[11];
//   char lon[11];    
//   
//   dtostrf(lati, 10, 10, latt);
//   dtostrf(longi, 10, 10, lon);
//
//   double i = 9.41516511;
//   String hola = String(i,10);
//   Serial.println(hola);
//   

//    String david = "Davidloca";
 
    String url = "/location?command=location&uptime=";
    url += String(millis());
    url += "&ip=";
    url += WiFi.localIP().toString();
    url += "&lat=";
    url += String(lati,8);
    url += "&long=";
    url += String(longi,8);

    Serial.println(gps.location.lat(),5);
    Serial.println(gps.location.lng(),5);
    Serial.println(gps.satellites.value());

    
//
//    Serial.println(latt);
//    Serial.println(lon);
    
    // Envoi la requete au serveur - This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
  
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  }


  
}

static void smartDelay(unsigned long ms)                // This custom version of delay() ensures that the gps object is being "fed".
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

