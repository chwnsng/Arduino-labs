#include <M5Stack.h>

void setup() {
  M5.begin();
  Serial.begin(9600);
  M5.Speaker.setVolume(1);
  M5.Speaker.update();
}

void loop() {
  if (Serial.available()>0) {
    int key=Serial.read();
    Serial.write(key);
    switch (key) {
      
    case 'c' : 
      M5.Speaker.tone(262);
      delay(1000); 
      M5.Speaker.mute();
      break;
       
    case 'd' : 
      M5.Speaker.tone(294);
      delay(1000); 
      M5.Speaker.mute();
      break;
      
    case 'e' : 
      M5.Speaker.tone(330);
      delay(1000); 
      M5.Speaker.mute();
      break;
         
    case 'f' : 
      M5.Speaker.tone(349);
      delay(1000); 
      M5.Speaker.mute();
      break;
         
    case 'g' : 
      M5.Speaker.tone(392);
      delay(1000); 
      M5.Speaker.mute();
      break;
         
    case 'a' : 
      M5.Speaker.tone(440);
      delay(1000); 
      M5.Speaker.mute();
      break;
         
    case 'b' : 
      M5.Speaker.tone(494);
      delay(1000); 
      M5.Speaker.mute();
      break;
      
    case 'C' : 
      M5.Speaker.tone(523);
      delay(1000); 
      M5.Speaker.mute();
      break;

    case 'D' : 
      M5.Speaker.tone(294*2);
      delay(1000); 
      M5.Speaker.mute();
      break;
      
    case 'E' : 
      M5.Speaker.tone(330*2);
      delay(1000); 
      M5.Speaker.mute();
      break;
         
    case 'F' : 
      M5.Speaker.tone(349*2);
      delay(1000); 
      M5.Speaker.mute();
      break;
         
    case 'G' : 
      M5.Speaker.tone(392*2);
      delay(1000); 
      M5.Speaker.mute();
      break;
         
    case 'A' : 
      M5.Speaker.tone(440*2);
      delay(1000); 
      M5.Speaker.mute();
      break;
         
    case 'B' : 
      M5.Speaker.tone(494*2);
      delay(1000); 
      M5.Speaker.mute();
      break;

    }
    delay(1300);
    //menu();
  }
  M5.update();
}

void menu() {
  Serial.println("\nExercise 3\nPlease enter note [c..a, C..A]");
  Serial.print("> ");
}
