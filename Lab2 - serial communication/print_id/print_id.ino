#include <M5Stack.h>

void setup() {
  M5.begin();
  Serial.begin(9600);
  Serial.println("Test print id");

}


void loop() {
  if (Serial.available()>0) {
    int key=Serial.read();
    Serial.write(key);
    switch(key){
      case 'a':
      Serial.println("Chawin Sungkhapong 6338040021");
      break;
    }
  }
}


  
  /*M5.Lcd.fillScreen(color[i]);
  i++;
  if (i >= (sizeof(color) / sizeof(int)))
  {
    i = 0;
  }
  delay(10);*/

/*void myInfo() {
  if (Serial.available()>0 {
    int key=Serial.read();
    if (key == "!"){
      Serial.print("Chawin Sungkhapong 6338040021");
    }
  }
}*/






  
