#include <M5Stack.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <Wire.h>
#include <SimpleDHT.h>
#define DHTPIN 5
SimpleDHT11 dht;
int temperature, humidity;

String LINE_TOKEN = "X28TRtYblhghFzBiqRPZ91YPLEqnr9Nu1iIJWsGpQ9U";

const char* ssid     = "PanusiPhone";   //Wifi Name
const char* password = "12345678";   //Wifi Password

void setup() {
  M5.begin();
  WiFi.begin(ssid, password);
  Wire.begin();
  Serial.begin(9600);
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
  // if the button state has changed:
  float temp, humid;
  int status = dht.read2(DHTPIN, &temp, &humid, NULL);
  if (status == SimpleDHTErrSuccess){
    temperature = temp;
    humidity = humid;
  }
  if (M5.BtnA.wasPressed()) {
      Line_Notify(LINE_TOKEN, "temp"+String (temperature)+"humid"+String(humid)); // Call funtion Line_Notify();
      delay(100);
  }
  M5.update();
}

void Line_Notify(String LINE_Token, String message) {

  String msg = String("message=") + message;

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
