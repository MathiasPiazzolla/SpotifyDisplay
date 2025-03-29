#include <XPT2046_Touchscreen.h>
#include <Adafruit_ILI9341.h>
#include <GothamBold12pt7b.h>

// display pins
#define TFT_CS 5
#define TFT_DC 4
#define TFT_RST 2
#define T_CS  22

#define ROTATION 0

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(T_CS);

void setup() {
  tft.begin();
  tft.setRotation(ROTATION);
  tft.fillScreen(ILI9341_BLACK);

  ts.begin();
  ts.setRotation(ROTATION);

  tft.fillRect(0, 0, 5, 5, ILI9341_RED);
  tft.fillRect(tft.width() - 5, tft.height() - 5, 5, 5, ILI9341_YELLOW);
}

void loop() {
  if (ts.touched()){
    int16_t x1, y1;
    uint16_t w, h;

    TS_Point p = ts.getPoint();

    String s = "x: " + String(p.x) + " y: " + String(p.y);
    
    tft.fillScreen(ILI9341_BLACK);
    tft.fillRect(0, 0, 5, 5, ILI9341_RED);
    tft.fillRect(tft.width() - 5, tft.height() - 5, 5, 5, ILI9341_YELLOW);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(1);
    tft.setFont(&GothamBold12pt7b);
    tft.getTextBounds(s, 0, 0, &x1, &y1, &w, &h);
    tft.setCursor((tft.width() - w) / 2, tft.height() / 2);
    tft.print(s.c_str());
  }
}
