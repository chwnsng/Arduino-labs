#include <M5Stack.h>
extern unsigned char logo[];
int screen_width = 320;
int screen_height = 240;
int raket_position1=0,raket_position2=5; //position of racket for each player.
long interval;
int ball_x;
int ball_y;
int ball_r;
int accel_x;
int accel_y;
int score1=0,score2=0,score; //score for each player


void repulse();
void update_score();

void start() {
  M5.Lcd.fillScreen(0x0000);
  interval = 60;
  ball_x = 160;
  ball_y = 120;
  ball_r = 6;
  accel_x = 5;
  accel_y = 5;
  score1 = 0;
  score2 =0;
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
  while (true)
  {
    int voltage = analogRead(36) * 3400 / 4096;
    int percentage = voltage * 100 / 3400;
    //raket_position = map(percentage, 0, 100, 0, 10);
    raket_position1 = map(percentage,0,100,0,5);
    if(M5.BtnA.wasPressed() && raket_position2<9){
      raket_position2++;
      Serial.println(raket_position2);
    }
    if(M5.BtnB.wasPressed() && raket_position2>5){
      raket_position2--;
      Serial.println(raket_position2);
    }
    if(M5.BtnC.wasPressed()){
      gameover();
    }
    int raket_width = 20;
    int raket_height = 40;
    int raket_margin = 10;
    int x, y, color, raket_x1, raket_y1,raket_x2, raket_y2;
    for (int i = 0; i < 10; i++)
    {
      if (i < 5)
      {
        x = 0;
        y = i * (raket_height + raket_margin);
      }
      else
      {
        x = screen_width - raket_width;
        y = (9 - i) * (raket_height + raket_margin);
      }    
      color = (i == raket_position1 || i == raket_position2) ? RED : WHITE;
      M5.Lcd.fillRect(x, y, raket_width, raket_height, color);

      
      if (i == raket_position1)   //determine x-y position of player 1's racket
      {
        raket_x1 = x;     
        raket_y1 = y;
      }
      else if(i == raket_position2) //determine x-y position of player 2's racket
      {
        raket_x2 = x;
        raket_y2 = y;
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

        if ((ball_x + ball_r >= raket_x1) && (ball_x-ball_r<=raket_width) && (ball_y + ball_r >= raket_y1) && (ball_y - ball_r <= raket_y1 + raket_height))
        {
          accel_x *= -1;
          accel_y *= 1;
          repulse();
        }
   
        if ((ball_x - ball_r >= raket_x2 - raket_width) && (ball_x +ball_r >= raket_x2)&& (ball_y + ball_r >= raket_y2) && (ball_y - ball_r <= raket_y2 + raket_height)) 
        {
          accel_x *= -1;
          accel_y *= 1;
          repulse();
        }
      }
      if ((ball_x < 0)   )
      {
        //gameover();
        //return;
        score2++;
        update_score();
        ball_x = 160;
        ball_y = 120;
        accel_x = 5;
        accel_y = 5;
        
      }
      else if ((ball_x > screen_width))
      {
        score1++;
        update_score();
        ball_x = 160;
        ball_y = 120;
        accel_x = 5;
        accel_y = 5;
        
      }
      M5.update();
    }
  
  }


void update_score(){
  M5.Lcd.fillScreen(0xffff);
  M5.Lcd.setCursor(120, 30);
  M5.Lcd.setTextColor(0x14);
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("score");
  M5.Lcd.setTextSize(8);
  M5.Lcd.setCursor(100, 100);
  M5.Lcd.print(score1);
  M5.Lcd.print(":");
  M5.Lcd.print(score2);
  delay(1000);
  M5.Lcd.fillScreen(0xffff);
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
  M5.Lcd.setCursor(95, 100);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(0xffff);
  M5.Lcd.println("GAME OVER");
  M5.Lcd.print("    player 1 = ");
  M5.Lcd.println(score1);
  M5.Lcd.print("    player 2 = ");
  M5.Lcd.print(score2);
  delay(4000);
  //ledBar(0, 0, 0, 12);
  /*for (int i = 0; i < score; i++)
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
  score1 = 0;
  score2 = 0;
  */
  start();


}
void setup(){
  M5.begin();
  pinMode(36, INPUT);
  pinMode(15, OUTPUT);
  digitalWrite (15, LOW);
  Serial.begin(9600);

}

void loop() {
  start();
  game();
}
