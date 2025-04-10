#include "SpotifyComponents.h"

uint16_t currentAverageColor;


SpotifyComponents::SpotifyComponents(int8_t TFT_CS, int8_t TFT_DC, int8_t TFT_RST)
  : Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST) {}

void SpotifyComponents::drawPlay() {
  fillCircle(width() / 2, height() - 40, 26, ILI9341_WHITE);
  fillRect(width() / 2 - 10, height() - 52, 6, 24, ILI9341_BLACK);
  fillRect(width() / 2 + 6, height() - 52, 6, 24, ILI9341_BLACK);
}

void SpotifyComponents::drawPause() {
  fillCircle(width() / 2, height() - 40, 26, ILI9341_WHITE);
  fillTriangle(
    width() / 2 - 9, height() - 52,
    width() / 2 + 12, height() - 52 + 12,
    width() / 2 - 9, height() - 52 + 24,
    ILI9341_BLACK);
}

void SpotifyComponents::drawProgressBar(int value) {
  fillRect(5, height() - 82, width() - 10 + 5, 6, ILI9341_BLACK);

  value = map(value, 0, 100, 10, width() - 10);
  fillRect(10, height() - 80, value - 10, 3, ILI9341_WHITE);
  fillRect(value, height() - 80, width() - 10 - value, 3, color565(70, 70, 70));

  fillCircle(value, height() - 79, 2, ILI9341_WHITE);
}

void SpotifyComponents::drawSkipArrows() {
  // skip left
  fillTriangle(
    65, height() - 40 - 20,
    30, height() - 40,
    65, height() - 40 + 20,
    ILI9341_WHITE);
  fillRect(29, height() - 40 - 20, 6, 40, ILI9341_WHITE);

  // skip right
  fillTriangle(
    width() - 65, height() - 40 - 20,
    width() - 30, height() - 40,
    width() - 65, height() - 40 + 20,
    ILI9341_WHITE);
  fillRect(width() - 34, height() - 40 - 20, 6, 40, ILI9341_WHITE);
}

void SpotifyComponents::drawTitle(const char* t) {
  setTextColor(ILI9341_WHITE);
  setTextSize(1);
  setFont(&GothamBold12pt7b);
  setCursor(10, height() - 110);
  setTextWrap(false);
  print(sanitizeString(t));
  fillRect(width() - 10, height() - 128, 10, 23, ILI9341_BLACK);
}
void SpotifyComponents::drawTitleHorizontal(const char* t,uint16_t currentAverageColor) {
  setTextColor(ILI9341_WHITE);
  setTextSize(1);
  setFont(&GothamBold12pt7b);
  setCursor(width() / 2 + 10, (height() / 100 * 20));
  setTextWrap(false);
  print(sanitizeString(t));
  fillRect((width()/2) - 10, height() - 128, 10, 23, currentAverageColor);
}


void SpotifyComponents::drawArtist(const char* a) {
  setTextColor(color565(70, 70, 70));
  setTextSize(1);
  setFont(&FreeSans9pt7b);
  setCursor(10, height() - 90);
  setTextWrap(false);
  print(sanitizeString(a));
  fillRect(width() - 10, height() - 102, 10, 14, ILI9341_WHITE);
}
void SpotifyComponents::drawArtistHorizontal(const char* a,uint16_t currentAverageColor) {
  setTextColor(color565(255, 255, 255));
  setTextSize(1);
  setFont(&FreeSans9pt7b);
  /*
  if (rig == false ) {
    setCursor(width() / 2, 127);
  }else {
    setCursor(width() / 2, 150);
  }
    */
  setCursor(width() / 2 + 10, 127);
  setTextWrap(false);
  print(sanitizeString(a));
  fillRect((width()/2) - 10, height() - 102, 10, 14, currentAverageColor);
}


void SpotifyComponents::drawCenterText(const char* s) {
  int16_t x1, y1;
  uint16_t w, h;

  fillScreen(ILI9341_BLACK);
  setTextColor(ILI9341_WHITE);
  setTextSize(1);
  setFont(&GothamBold12pt7b);
  getTextBounds(s, 0, 0, &x1, &y1, &w, &h);
  setCursor((width() - w) / 2, height() / 2);
  print(s);
}




char* SpotifyComponents::sanitizeString(const char* str) {
  String s = str;
  char* out = (char*)malloc(sizeof(char) * s.length());
  s.replace("è", "e");
  s.replace("é", "e");
  s.replace("È", "e");
  s.replace("à", "a");
  s.replace("å", "a");
  s.replace("Å", "a");
  s.replace("ù", "u");
  s.replace("Ù", "u");
  s.replace("ò", "o");
  strcpy(out, s.c_str());
  return out;
}