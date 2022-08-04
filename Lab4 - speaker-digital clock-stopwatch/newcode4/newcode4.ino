#include <M5Stack.h>

#define TFT_GREY 0x5AEB

uint32_t targetTime = 0;                    // for next 1 second timeout

static uint8_t conv2d(const char* p); // Forward declaration needed for IDE 1.6.x

uint8_t hh = 0,mm =0, ss = 0, sss = 0;

byte omm = 99, oss = 99, osss = 99;
byte xcolon = 0, xsecs = 0, xms = 0;
unsigned int colour = 0;

void setup(void) {
  //Serial.begin(115200);
  M5.begin();
  // M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(TFT_BLACK);

  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);

  targetTime = millis() + 10;
}

void loop() {
  if(M5.BtnA.wasPressed() ){
    while(true){
  if (targetTime < millis()) {
    // Set next update for 1 second later
    targetTime = millis() + 10;

    // Adjust the time values by adding 1 second
    sss++;              // Advance second
    if (sss == 100){
      sss = 0;
      osss = ss;
      ss++;
      if (ss > 59) {    // Check for roll-over
       ss = 0;          // Reset seconds to zero
       omm = mm;        // Save last minute time for display update
       mm++;            // Advance minute
       if (mm > 59) {   // Check for roll-over
        mm = 0;
        hh++;          // Advance hour
        if (hh > 23) { // Check for 24hr roll-over (could roll-over on 13)
          hh = 0;      // 0 for 24 hour clock, set to 1 for 12 hour clock
        }
      }
     }
    }

    // Update digital time
    int xpos = 0;
    int ypos = 85; // Top left corner ot clock text, about half way down

    if (omm != mm) { // Redraw hours and minutes time every minute
      omm = mm;
      // Draw hours and minutes
      if (hh < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 6); // Add hours leading zero for 24 hr clock
      xpos += M5.Lcd.drawNumber(hh, xpos, ypos, 6);             // Draw hours
      xcolon = xpos; // Save colon coord for later to flash on/off later
      xpos += M5.Lcd.drawChar(':', xpos, ypos, 6);
      if (mm < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 6); // Add minutes leading zero
      xpos += M5.Lcd.drawNumber(mm, xpos, ypos, 6);             // Draw minutes
      xsecs = xpos; // Set seconds 'x' position for later display updates
    }
    if (oss != ss) { // Redraw seconds time every second
      oss = ss;
      xpos = xsecs;

      if (ss % 2) { // Flash the colons on/off
        M5.Lcd.setTextColor(0x39C4, TFT_BLACK);        // Set colour to grey to dim colon
        M5.Lcd.drawChar(':', xcolon, ypos, 6);     // Hour:minute colon
        xpos += M5.Lcd.drawChar(':', xsecs, ypos, 6); // Seconds colon
        M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
      }
      else {
        M5.Lcd.drawChar(':', xcolon, ypos, 6);     // Hour:minute colon
        xpos += M5.Lcd.drawChar(':', xsecs, ypos, 6); // Seconds colon
      }

      //Draw seconds
      if (ss < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 6); // Add leading zero
      xpos += M5.Lcd.drawNumber(ss, xpos, ypos, 6);                     // Draw seconds
      xms = xpos;
    }

    if (osss != sss) { // Redraw seconds time every second
      osss = sss;
      xpos = xms;

      xpos += M5.Lcd.drawChar(':', xpos, ypos, 6); // Milliseconds colon

      if (ss < 100) xpos += M5.Lcd.drawChar('0', xpos, ypos, 6); // Add leading zero
      M5.Lcd.drawNumber(sss, xpos, ypos, 6);                     // Draw seconds
    }
  }
  if(M5.BtnB.wasPressed() )break;
  M5.update();
  }
 }
 if (M5.BtnC.wasPressed()) {
  hh = 0;
  mm = 0;
  ss = 0;
  sss = 0;
  omm = 99;
  oss = 99;
  osss = 99;
  int xpos = 0;
  int ypos = 85;

  xpos += M5.Lcd.drawChar('0', xpos, ypos, 6);
  xpos += M5.Lcd.drawChar('0', xpos, ypos, 6);
  xpos += M5.Lcd.drawChar(':', xpos, ypos, 6);
  xpos += M5.Lcd.drawChar('0', xpos, ypos, 6);
  xpos += M5.Lcd.drawChar('0', xpos, ypos, 6);
  xpos += M5.Lcd.drawChar(':', xpos, ypos, 6);
  xpos += M5.Lcd.drawChar('0', xpos, ypos, 6);
  xpos += M5.Lcd.drawChar('0', xpos, ypos, 6);
  xpos += M5.Lcd.drawChar(':', xpos, ypos, 6);
  xpos += M5.Lcd.drawChar('0', xpos, ypos, 6);
  xpos += M5.Lcd.drawChar('0', xpos, ypos, 6);
  xpos += M5.Lcd.drawChar('0', xpos, ypos, 6);

 }

 M5.update();

 }
