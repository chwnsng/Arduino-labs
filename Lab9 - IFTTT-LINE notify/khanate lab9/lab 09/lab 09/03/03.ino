#include <SimpleDHT.h>
#include <Wire.h>
#include <M5Stack.h>

#define DHTPIN 5
SimpleDHT11 dht;
int temperature, humidity;

/*
 * Rui Santos 
 * Complete Project Details http://randomnerdtutorials.com
 */
 
#include <WiFi.h>

// Replace with your SSID and Password
const char* ssid     = "you better pay me";
const char* password = "spaghetti@51";

// Replace with your unique IFTTT URL resource
const char* resource = "/trigger/Panus2544/with/key/bypXr7vwl7oTTWarD9aeNp";

// How your resource variable should look like, but with your own API KEY (that API KEY below is just an example):
//const char* resource = "/trigger/bme280_readings/with/key/nAZjOphL3d-ZO4N3k64-1A7gTlNSrxMJdmqy3";

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

// Time to sleep
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// sleep for 30 minutes = 1800 seconds
uint64_t TIME_TO_SLEEP = 30;

// Uncomment to use BME280 SPI
/*#include <SPI.h>
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10*/

#define SEALEVELPRESSURE_HPA (1013.25)

//Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

void setup() {
  M5.begin();
  Serial.begin(115200); 
  delay(2000);
  // initialize BME280 sensor
  Wire.begin();

  initWifi();
}

void loop() {
  float temp, humid;
  int status = dht.read2(DHTPIN, &temp, &humid, NULL);
  if (status == SimpleDHTErrSuccess){
    temperature = temp;
    humidity = humid;
  }
  if (M5.BtnA.wasPressed()){
    makeIFTTTRequest(temperature, humidity);
    Serial.println(temperature);
    Serial.println(humidity);
  }
  delay(500);
  M5.update();
}

// Establish a Wi-Fi connection with your router
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
  Serial.print("Connecting to "); 
  Serial.print(server);
}

// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest(int temperature, int humidity) {  
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(resource);
  // Temperature in Celsius
  String jsonObject = String("{\"value1\":\"") + temperature + "\",\"value2\":\"" + humidity + "\"}";
                      
  // Comment the previous line and uncomment the next line to publish temperature readings in Fahrenheit                    
  /*String jsonObject = String("{\"value1\":\"") + (1.8 * bme.readTemperature() + 32) + "\",\"value2\":\"" 
                      + (bme.readPressure()/100.0F) + "\",\"value3\":\"" + bme.readHumidity() + "\"}";*/
                      
  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
        
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
}
