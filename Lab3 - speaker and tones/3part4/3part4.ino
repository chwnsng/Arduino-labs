#include <M5Stack.h>
int i = 0;
const int nC4 = 262;
const int nD4 = 294;
const int nE4 = 330;
const int nF4 = 349;
const int nG4 = 392;
const int nA4 = 440;
const int nB4 = 494;
const int nC5 = 524;

const int note[] = {  
  nE4,nE4,nE4,nE4,nE4,nE4,nE4,nG4,nC4,nD4,nE4,nF4,nF4,nF4,nF4,nF4,nE4,nE4,nE4,nD4,nD4,nE4,nD4,nG4};
void setup() {
  M5.begin();
  M5.Speaker.setVolume(1);
  M5.Speaker.update();
}

void loop() {
  M5.update();
    if (M5.BtnA.wasPressed()) {
      M5.Speaker.tone(note[i]);
      i = i + 1;
    }
    if (M5.BtnA.wasReleased()){
      M5.Speaker.mute();
    }
    delay(50);
  }
