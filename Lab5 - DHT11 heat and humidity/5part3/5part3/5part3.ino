//5part4

#include <M5Stack.h>
#include <SimpleDHT.h>
#include <Wire.h>

#define DHTPIN 5
SimpleDHT11 dht;
int temperature, humidity;
int old_t = -1000;
int old_h = -1;
boolean pressed = false;
boolean toggle = false;

void setup(){
    Serial.begin(115200);
    M5.begin();
    Wire.begin();
    M5.Lcd.setTextSize(10);
}

void loop() {
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
    old_t = temperature;
    old_h = humidity;
    M5.update();
}
