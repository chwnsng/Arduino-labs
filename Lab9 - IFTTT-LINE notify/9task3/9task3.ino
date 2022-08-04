#include <WiFi.h> //https://github.com/esp8266/Arduino
#include <SimpleDHT.h>
#include <WiFiClientSecure.h>
#include <M5Stack.h>
#include <Wire.h>

#define DHTPIN 5
SimpleDHT11 dht;
int temperature, humidity;

String LINE_TOKEN = "sFerXXkShxE93xWglt2oCcLW4WT8s2qhx4W6sXOVdpR";

const char* ssid     = "you better pay me";   //Wifi Name
const char* password = "spaghetti@51";   //Wifi Password


void setup() {

  WiFi.begin(ssid, password);
  Serial.begin(115200);
  M5.begin();
  Wire.begin();
  initWifi();

}


void loop() {
  float temp, humid;
  int status = dht.read2(DHTPIN, &temp, &humid, NULL);
  if (status == SimpleDHTErrSuccess) {
    temperature = temp;
    humidity = humid;
  }
  Line_Notify(LINE_TOKEN, "Temperature = " + String(temperature) + "\nHumidity = " + String(humidity));
  delay(3000); 
}



void Line_Notify(String LINE_Token, String message) {

  String msg = String("message= ") + message;

  WiFiClientSecure client;
  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Authorization: Bearer " + String(LINE_Token) + "\r\n";
  req += "Content-Length: " + String(msg.length()) + "\r\n";
  req += "\r\n";
  req +=  msg;

  client.print(req);

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}

void initWifi() {
  Serial.print("Connecting to: "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);  

  int timeout = 10 * 4; // 10 seconds
  while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if(WiFi.status() != WL_CONNECTED) {
     Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: "); 
  Serial.print(millis());
  Serial.print(", IP address: "); 
  Serial.println(WiFi.localIP());
}
