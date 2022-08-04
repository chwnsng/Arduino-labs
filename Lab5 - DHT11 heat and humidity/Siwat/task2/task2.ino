#include <Arduino.h>
#include <M5Stack.h>
#include <Wire.h>
#include <SimpleDHT.h>
#define DHTPIN 5
boolean f = false;
boolean force_update=false;
SimpleDHT11 dht;
int temperature = 0;
int humidity = 0;
int old_t = -1000;
int old_h = 0;
void setup()
{
  Serial.begin(115200);
  M5.begin();
  Wire.begin();
  M5.Lcd.setTextSize(10);
}

void loop()
{
  float temp, humid;
  int status = dht.read2(DHTPIN, &temp, &humid, NULL);
  if (status == SimpleDHTErrSuccess)
  {
    temperature = (int)temp;
    humidity = (int)humid;
  }
  if (temperature != old_t || force_update || humidity!=old_h)
  {
    force_update=false;
    if (temp > 27)
      M5.Lcd.setTextColor(0xe8e4);
    else
      M5.Lcd.setTextColor(0x2589);
    M5.Lcd.fillScreen(0);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print(f?(int)(temperature*(9.0/5.0))+32:temperature);
    M5.Lcd.println(f?" F":" C");
    M5.Lcd.print(humidity);
    M5.Lcd.print(" %");
  }
  if(M5.BtnA.wasPressed()){
    force_update=true;
    f=!f;
  }
  old_t=temperature;
  old_h=humidity;
  M5.update();
}
