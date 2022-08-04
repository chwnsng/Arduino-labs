#include <Arduino.h>
#include <M5Stack.h>
#define TFT_GREY 0x5AEB
long currenttime = 0;
long targetTime = 0;
boolean timerRun = false;
boolean firstRun = true;
long a, b, c;
boolean catcher = false;
void DrawScreen(long sec)
{
    //TimeCode Construction
    M5.Lcd.setCursor(60, 100);
    M5.Lcd.setTextSize(10);
    M5.Lcd.printf("%02d %02d", ((sec - (sec % 60)) / 60), (sec % 60));
    //Cursor Construction
    M5.Lcd.setCursor(145, 100);
    M5.Lcd.setTextSize(10);
    M5.Lcd.print(":");
}
void setup()
{
    M5.begin();
    M5.Lcd.clear();
    M5.Lcd.fillScreen(TFT_WHITE);
    M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
    //Hot Tips
    M5.Lcd.setCursor(55, 210);
    M5.Lcd.setTextSize(2);
    M5.Lcd.print("M       S     Toggle");
    M5.Lcd.setTextSize(1);
    Serial.begin(115200);
    DrawScreen(0);
}
void loop()
{
    if (M5.BtnA.wasPressed())
        a = millis();
    if (M5.BtnB.wasPressed())
        b = millis();
    if (M5.BtnC.wasPressed())
        c = millis();
    if (M5.BtnA.wasReleased() && !timerRun)
    {
        if (millis() - a >= 3000)
        {
            currenttime = currenttime % 60;
        }
        else
        {
            currenttime += 60;
            firstRun = false;
        }
        DrawScreen(currenttime);
        if (catcher)catcher = false;
    
    }
    if (M5.BtnB.wasReleased() && !timerRun)
    {
        if (millis() - b >= 3000)
        {
            currenttime = currenttime - (currenttime % 60);
        }
        else
        {
            currenttime += 1;
            firstRun = false;
        }
        DrawScreen(currenttime);
        if (catcher)catcher = false;
    }
    if (M5.BtnC.wasReleased())
    {
        if (millis() - c >= 3000)
        {
            timerRun = false;
            firstRun = true;
            currenttime = 0;
            DrawScreen(currenttime);
        }
        else if (!catcher)
        {
            timerRun = !timerRun;
            if (currenttime==0&&timerRun)timerRun=!timerRun;
        }
        if (catcher)catcher = false;
    }
    if (currenttime <= 0 && !firstRun && timerRun)
    {
        DrawScreen(0);
        M5.Speaker.tone(3000);
        while (!firstRun)
        {
            if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed())
            {
                catcher = true;
                break;
            }
            M5.update();
        }
        M5.Speaker.mute();
        firstRun = true;
        timerRun = false;
    }
    if (targetTime < millis() && timerRun && !firstRun)
    {
        targetTime = millis() + 1000;
        currenttime--;
        DrawScreen(currenttime);
    }
    M5.update();
}
