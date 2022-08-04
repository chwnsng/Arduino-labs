#include <M5Stack.h>

const int C4 = 262;
const int D4 = 294;
const int E4 = 330;
const int F4 = 349;
const int G4 = 392;
const int A4 = 440;
const int B4 = 494;
const int C5 = 524;
const int D5 = D4*2;

//const int zero = 0;

const int note[] = {
  D5,C5,D5,F4,C5,A4,A4,A4
};

const int duration[] = {
  4,4,4,4,4,4,4,1
};

void setup() {
  M5.begin();
  M5.Speaker.setVolume(1);
  M5.Speaker.update();
}


void loop() {
  for (int i=0; i<8; i++) {
    int wait = 1000/duration[i];
    M5.Speaker.tone(note[i]);
    delay(wait);
    M5.Speaker.mute();
    delay(50);
  }
  M5.Speaker.mute();
  delay(2000);
  M5.update();
  
}
