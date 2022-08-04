#include <M5Stack.h>
#include <SimpleDHT.h>
#include <Wire.h>

#define DHTPIN 5
SimpleDHT11 dht;
int temperature, humidity;
int old_t = -1000;
int old_h = -1;
long lastmillis = 0;
boolean pressed = false;
boolean toggle = false;
boolean pressb = false;

//Time
uint32_t targetTime = 0; 


static uint8_t conv2d(const char* p); 
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3); 
uint8_t ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time


void setup(){
    Serial.begin(115200);
    M5.begin();
    Wire.begin();
    M5.Lcd.setTextSize(10);
}

void loop() {


    //time
    if(millis()-lastmillis>=60000){
      mm++;
      if(mm>=60){
        hh++;
        mm=0;
      }
      toggle = true;
      lastmillis=millis();
    }

  
    float temp,humid;
    int status = dht.read2(DHTPIN, &temp, &humid, NULL);
    
    if (status == SimpleDHTErrSuccess) {
        temperature = temp;
        humidity = humid;
    }
    if (temperature != old_t || humidity != old_h || toggle){
        toggle = false;
        if (temp > 27){
            M5.Lcd.setTextColor(0xe8e4);
        }
        else{
            M5.Lcd.setTextColor(0x2589);
        }
        M5.Lcd.fillScreen(0);
        M5.Lcd.setCursor(0,0);
        
        if (pressed) {
          M5.Lcd.print(temperature*1.8 + 32);
          M5.Lcd.print(" F\n");
        } else {
          M5.Lcd.print((float)temperature);
          M5.Lcd.print(" C\n");
        }
        
        M5.Lcd.print(humidity);
        M5.Lcd.print(" %");
    }
    if (M5.BtnA.wasPressed()){
      toggle = true;
      pressed = !pressed;
    }

    if (M5.BtnB.wasPressed()){
      pressb = !pressb;
      toggle = true;
    }

    
    //Show time
    if (pressb){
      M5.Lcd.setCursor(0,160);
      M5.Lcd.printf("%02d:%02d%s", hh % 12, mm, (hh % 24) < 12 ? "AM" : "PM");
    } else {
      M5.Lcd.setCursor(0,160);
      M5.Lcd.printf("%02d:%02d", hh % 24, mm);
    }

    
    old_t = temperature;
    old_h = humidity;
    M5.update();
}


// function to extract number from compile string (don’t need to understand this for now)
static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}
