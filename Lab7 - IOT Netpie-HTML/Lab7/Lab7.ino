#include <MicroGear.h>
#include <M5Stack.h>
#include <SimpleDHT.h>

const char* ssid     = "you better pay me";
const char* password = "spaghetti";

#define APPID   "fusili"
#define KEY     "MsBgQFHZIQ4VyPw"
#define SECRET  "jBq6RJK7EbpExsyEuOEOMoPFt"

#define ALIAS   "esp32"
#define TargetWeb "HTML_web"

#define D4 5   // TXD1
#define DHTPIN D4     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11

SimpleDHT11 dht;

WiFiClient client;
MicroGear microgear(client);

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) 
{
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
}


void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) 
{
    Serial.println("Connected to NETPIE...");
    microgear.setAlias(ALIAS);
}

void setup() 
{
     /* Event listener */
    microgear.on(MESSAGE,onMsghandler);
    microgear.on(CONNECTED,onConnected);

    //dht.begin();
    Serial.begin(115200);
    Serial.println("Starting...");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
       delay(250);
       Serial.print(".");
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    microgear.init(KEY,SECRET,ALIAS);
    microgear.connect(APPID);
}

void loop() 
{
    if (microgear.connected())
    {
       microgear.loop();
       Serial.println("connected");

       float Humidity = 20.0;
       float Temp = 10.0; // Read temperature as Celsius (the default)
       String data = "/" + String(Humidity) + "/" + String(Temp);
       char msg[128];
       data.toCharArray(msg,data.length());
       Serial.println(msg);    

       microgear.chat(TargetWeb , msg);
    }
   else 
   {
    Serial.println("connection lost, reconnect...");
    microgear.connect(APPID);
   }
    delay(1500);
}
