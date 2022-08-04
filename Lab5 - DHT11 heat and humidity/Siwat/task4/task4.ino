#include <Arduino.h>
#include <M5Stack.h>
#include <Wire.h>
#include <SimpleDHT.h>
#include <Ticker.h>
#define DHTPIN 5
boolean f = false;
boolean force_update = false;
long lastmillis = 0;
boolean ampm = false;
SimpleDHT11 dht;
static uint8_t conv2d(const char *p);
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3);
uint8_t ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time
int temperature = 0;
int humidity = 0;
int old_t = -1000;
int old_h = 0;
void setup()
{
  M5.begin();
  Serial.begin(115200);
  Wire.begin();
  pinMode(DHTPIN, INPUT);
  M5.Lcd.setTextSize(10);
}

void loop()
{
  if(millis()-lastmillis>=60000){
    mm++;
    if(mm>=60){
      hh++;
      mm=0;
    }
    force_update= true;
    lastmillis=millis();
  }
  float temp, humid;
  int status = dht.read2(DHTPIN, &temp, &humid, NULL);

  if (status == SimpleDHTErrSuccess)
  {
    temperature = (int)temp;
    humidity = (int)humid;
  }
  if (temperature != old_t || force_update || humidity != old_h)
  {
    M5.Lcd.clear();
    force_update = false;
    if (temp > 27)
      M5.Lcd.setTextColor(0xe8e4);
    else
      M5.Lcd.setTextColor(0x2589);
    M5.Lcd.fillScreen(0);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print(f ? (int)(temperature * (9.0 / 5.0)) + 32 : temperature);
    M5.Lcd.println(f ? " F" : " C");
    M5.lcd.print(humidity);
    M5.Lcd.print(" %");
  }
  if (M5.BtnA.wasPressed())
  {
    force_update = true;
    f = !f;
  }
  if (M5.BtnB.wasPressed())
  {
    force_update = true;
    ampm = !ampm;
  }
  //TimeCode Construction
  //M5.Lcd.setCursor(0, 130);
  M5.Lcd.setTextSize(10);
  if (ampm)
    M5.Lcd.printf("%02d:%02d%s", hh % 12, mm, (hh % 24) < 12 ? "AM" : "PM");
  else
    M5.Lcd.printf("%02d:%02d", hh % 24, mm);
  old_t = temperature;
  old_h = humidity;
  M5.update();
}
static uint8_t conv2d(const char *p)
{
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}
