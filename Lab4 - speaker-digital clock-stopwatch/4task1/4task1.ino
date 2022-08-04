#include <M5Stack.h>

const int D0 = -1;
const int Cs1 = 277;
const int D1 = 294;
const int Ds1 = 311;
const int E1 = 330;
const int F1 = 349;
const int Fs1 = 370;
const int note_G1 = 392;
const int Gs1 = 415;
const int A1 = 440;
const int As1 = 466;
const int C2 = 494;

int music[] = 
{
  E1, E1, E1,
  E1, E1, E1,
  E1, note_G1, Cs1, D1, E1,
  F1, F1, F1, F1, F1,
  F1, E1, E1,
  E1, D1, D1, E1, D1,
  note_G1
};

int t = 0;

void setup() {
  M5.begin();
  M5.Speaker.setVolume(1);
  M5.Speaker.update();
  M5.Speaker.mute();
}

void loop() {
  if (M5.BtnA.wasPressed())
  {
    if (t == ((sizeof(music) / sizeof(int)) - 1)){
      t = 0;
    }
    M5.Speaker.tone(music[t]);
  }
  if (M5.BtnA.wasReleased()){
    M5.Speaker.mute();
    t++;
  }
  M5.update();
}
