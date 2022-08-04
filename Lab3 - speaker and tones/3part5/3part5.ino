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
const int E5 = E4*2;
const int F5 = F4*2;

const int note[] = {
  G4,G4,A4,G4,C5,B4,  G4,G4,A4,G4,D5,C5, G4,G4,G4,E4,C5,C5,B4,A4,  F5,F5,E5,C5,D5,C5
};
const int duration[] = {
  4,4,3,3,3,2,  4,4,3,3,3,2,  4,4,3,3,4,4,3,3,  4,4,3,3,3,3};

void setup() {
  M5.begin();
  Serial.begin(9600);
  M5.Speaker.setVolume(1);
  M5.Speaker.update();
}


int multiply = 1;
int i=0;

int wait = 250;
void loop() {
  i=0;
  for (i=0;i<27;i++) {

    M5.Speaker.tone(note[i]);
    delay(wait);
    M5.Speaker.mute();
    delay(50);
    if (M5.BtnA.wasPressed()) {
    wait -= 100 ;
    }

    
    if (M5.BtnB.wasPressed()) {
    wait += 100; 
    }


    if (M5.BtnC.wasPressed()) {
    wait = 250;
    }
    M5.update();
    
  }
}
