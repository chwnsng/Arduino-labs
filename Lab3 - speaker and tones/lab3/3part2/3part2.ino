#include <M5Stack.h>

void setup() {
  M5.begin();
  M5.Speaker.setVolume(1);
  M5.Speaker.update();
  M5.Speaker.mute();
  Serial.begin(9600);
  Serial.println("MAET");

}

int reading = 0;
void loop() {
  if (Serial.available()>0) {
    reading = Serial.read();
    //Serial.print(reading);
    Serial.print(" - HEX: 0x");
    Serial.print(reading,HEX);
  }
  
  if (M5.BtnA.isPressed()) {
    Serial.println("Button A is Pressed"); 
  }
  M5.update();

  if (M5.BtnA.isPressed()) {
    Serial.println("Button A is Pressed"); 
  }
  M5.update();
  

}
