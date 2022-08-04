
#include <ThingSpeak.h>
#include<WiFi.h>
#include <WiFiClient.h>

#define triggerPin 8
#define echoPin 5
#define LEDpin 2



// Network information
char ssid[] = "mehomeais_2.4G";
char password[] = "home71160";

// ThingSpeak information
char* writeAPIKey = "YORU3A9BHWWLMXYQ";
char* readAPIKey = "SY0IJA41UA41HGBU"; 
const long channelID = 1363547; 
const unsigned int firstReadFieldNumber = 2;
const unsigned int secondReadFieldNumber = 3; 
const unsigned int switchField = 4; // Field number (1-8) to use to change status of device.  Determines if data is read from ThingSpeak.

// Other constants
const unsigned long postingInterval = 60L * 1000L;   // Post data every 60 seconds

// Global variables
unsigned long lastConnectionTime = 0;
long lastUpdateTime = 0;
float distanceThreshold = 0;
bool getInfo = 1;  // Set this to zero if you don’t want to read data from ThingSpeak anymore (i.e. calibration complete)
int points = 7; 
WiFiClient client; 



void setup() {
  Serial.begin(9600);
  pinMode(triggerPin, OUTPUT); 
  pinMode(LEDpin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  //connectWifi();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("Start");

  // Get the initial parameters from ThingSpeak.
  distanceThreshold = readTSData(channelID,firstReadFieldNumber,readAPIKey); 
  points = readTSData(channelID,secondReadFieldNumber,readAPIKey);   
}

void loop() {
  
  float  distance=0;

  // Make sure there is an Internet connection.
  if(WiFi.status() != WL_CONNECTED){
    
    connectWifi();  
    }

     for (uint16_t loops = 0; loops < points; loops++){
      distance += getDistance(triggerPin,echoPin);  //make a measurement, store the sum of all measurements
      delay(5);  
     }
     
     distance = distance/points;
     
    if (distance < distanceThreshold){
      digitalWrite(LEDpin,HIGH);
      }
    
     else{
      
      digitalWrite(LEDpin,LOW); 
      }
      
     
     Serial.println("Ave: "+ String(distance)+ " cm");
      
    if (millis() - lastUpdateTime >=  postingInterval) {  
      
      lastUpdateTime = millis();
      
           if (!(getInfo==0)){
            
            distanceThreshold = readTSData(channelID,firstReadFieldNumber,readAPIKey);
            points = readTSData(channelID,secondReadFieldNumber,readAPIKey);
            getInfo = (bool)readTSData(channelID,switchField,readAPIKey);
            }
            
       if (distance < distanceThreshold){
        
      // Write data to ThingSpeak channel.
       writeTSData(channelID, 1, distance, writeAPIKey);
       }
       
      }
      
    delay(500);    // Provide some delay between measurements.
    
}



int writeTSData(long TSChannel,unsigned int TSField,float data,char* ReadAPIKey){
  int  writeSuccess = ThingSpeak.writeField(TSChannel, TSField, data, writeAPIKey); //write the data to the channel
  return writeSuccess;
}

// Use this function if you want multiple fields simultaneously.

/*int writeTDData(long TSChannel,unsigned int TSField1,float data1,unsigned int TSField2,data2,char* ReadAPIKey){
  ThingSpeak.setField(TSField1,data1);
  ThingSpeak.setField(TSField1,data2);
   
  writeSuccess = ThingSpeak.writeFields(TSChannel, writeAPIKey);
  return writeSuccess;
}
*/
 
float readTSData(long TSChannel,unsigned int TSField,char* ReadAPIKey){
  
  float data = 0;
  
  data = ThingSpeak.readFloatField(TSChannel,TSField,ReadAPIKey);
  Serial.println(" Data read from ThingSpeak "+String(data));
  return data;
}

float getDistance(int tPin,int ePin){
  
  long duration, distance;
    
  digitalWrite(tPin, LOW);  // Reset the trigger pin.
  delayMicroseconds(2); 
  digitalWrite(tPin, HIGH);  // Start a measurement.
  delayMicroseconds(10); // 
  digitalWrite(tPin, LOW);   // Complete the pulse.
  duration = pulseIn(ePin, HIGH);  // Wait for a reflection pulse.
  distance = (duration/2) / 29.1;     // Calculate the appropriate distance using the estimated speed of sound.

  // This section is useful when debugging the sensor.
/*
  if (distance >= 200 || distance <= 0){
    Serial.println("Out of range");
  }
  else {
   Serial.print(distance);
   Serial.println(" cm");
  }
  */
  return distance;
}

int connectWifi(){
   
  while (WiFi.status() != WL_CONNECTED) {
     WiFi.begin(ssid, password);
     delay(2500);
     Serial.println("Connecting to WiFi");
  }
  
  Serial.println("Connected");
  ThingSpeak.begin(client);
}
