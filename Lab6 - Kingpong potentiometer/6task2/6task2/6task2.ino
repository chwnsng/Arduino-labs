#include <M5Stack.h>

extern unsigned char logo[];
int screen_width = 320;
int screen_height = 240;
int raket_position, raket_position2 = 5;
long interval;
int ball_x;
int ball_y;
int ball_r;
int accel_x;
int accel_y;
int score;

void start() {
  M5.Lcd.fillScreen(0x0000);
  interval = 60;
  ball_x = 160;
  ball_y = 120;
  ball_r = 6;
  accel_x = 5;
  accel_y = 5;
  score = 0;
  M5.Lcd.drawBitmap(0, 0, screen_width, screen_height, (uint16_t *)logo);
  M5.Lcd.setTextSize(2);
  long previousMillis = 0;
  long interval = 500;
  int color = 0x0000;
  while(true)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      color = (color == 0x7bef) ? 0xffff : 0x7bef;
      M5.Lcd.setCursor(35, 200);
      M5.Lcd.setTextColor(color);
      M5.Lcd.print("press any key to play");
    }
    M5.update();
    if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) break;
  }
  M5.Lcd.fillScreen(0x0000);
  M5.Speaker.tone(800);
  delay(100);
  M5.Speaker.tone(1200);
  delay(100);
  M5.Speaker.mute();
  delay(500);
  M5.Lcd.fillScreen(0xffff);
}

void game() {
  M5.Lcd.fillCircle(ball_x, ball_y, ball_r, 0x7bef);
  long previousMillis = 0;
  while (true) {
    int voltage = analogRead(36) * 3400 / 4096;
    int percentage = voltage * 100 / 3400;
    raket_position = map(percentage, 0, 100, 0, 10);
    if (M5.BtnA.wasPressed()){
      raket_position++;
    }else if (M5.BtnB.wasPressed()){
      raket_position--;
    }
    int raket_width = 20;
    int raket_height = 40;
    int raket_margin = 10;
    int x, y, color, raket_x, raket_y, raket_x2, raket_y2;
    for (int i = 0; i < 10; i++){
      if (i < 5){
        x = 0;
        y = i * (raket_height + raket_margin);
      }
      else {
        x = screen_width - raket_width;
        y = (9 - i) * (raket_height + raket_margin);
      }    
      color = (i == raket_position || i == raket_position2) ? RED : WHITE;
      M5.Lcd.fillRect(x, y, raket_width, raket_height, color);
      if (i == raket_position){
        raket_x = x;
        raket_y = y;
      }
    }
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      M5.Lcd.fillCircle(ball_x, ball_y, ball_r, WHITE);
      ball_x += accel_x;
      ball_y += accel_y;
      M5.Lcd.fillCircle(ball_x, ball_y, ball_r, 0x7bef);
      if (ball_y <= 0)
        accel_y *= -1;
      if (ball_y >= 240)
        accel_y *= -1;    
      if (raket_position > 4){
        if ((ball_x + ball_r >= raket_x) && (ball_y + ball_r >= raket_y) && (ball_y - ball_r <= raket_y + raket_height)){
          accel_x *= -1;
          accel_y *= 1;
          repulse();
        }
      }
      else{
        if ((ball_x - ball_r <= raket_x + raket_width) && (ball_y + ball_r >= raket_y) && (ball_y - ball_r <= raket_y + raket_height)) {
          accel_x *= -1;
          accel_y *= 1;
          repulse();
        }
      }
      if ((ball_x < 0) || (ball_x > screen_width) || (ball_y < 0) || (ball_y > screen_height)){
        gameover();
        return;
      }
    }
  }
}




void repulse() {
  M5.Speaker.tone(1800);
  delay(50);
  M5.Speaker.mute();
  score++;
  interval -= (interval >= 25) ? 5 : 0;
}

void gameover() {
  M5.Lcd.fillScreen(0x0000);
  M5.Speaker.tone(1200);
  delay(100);
  M5.Speaker.tone(1000);
  delay(100);
  M5.Speaker.tone(800);
  delay(100);
  M5.Speaker.tone(1100);
  delay(100);
  M5.Speaker.mute();
  M5.Lcd.setCursor(100, 100);
  M5.Lcd.setTextColor(0xffff);
  M5.Lcd.print("GAME OVER");
  delay(1000);
  for (int i = 0; i < score; i++)
  {
    M5.Lcd.setCursor(100, 120);
    M5.Lcd.setTextColor(0xffff);
    M5.Lcd.print("SCORE: ");
    M5.Lcd.setTextColor(0x0000);
    M5.Lcd.print((i < 1) ? 0 : i);
    M5.Lcd.setCursor(100, 120);
    M5.Lcd.setTextColor(0xffff);
    M5.Lcd.print("SCORE: ");
    M5.Lcd.print(i + 1);
    M5.Speaker.tone(1200);
    delay(100);
    M5.Speaker.mute();
  }
  delay(2000);
}

void setup(){
  M5.begin();
  pinMode(36, INPUT);
}

void loop() {
  start();
  game();
}
