#include <M5Stack.h>

void setup() {
  M5.begin();
  M5.Speaker.setVolume(0);
  M5.Speaker.update();
  Serial.begin(9600);

}

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
