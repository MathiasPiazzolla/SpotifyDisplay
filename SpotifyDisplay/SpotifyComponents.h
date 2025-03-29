#include <Adafruit_ILI9341.h>
#include "GothamBold12pt7b.h"  // non ho caratteri tipo la è
#include <Fonts/FreeSans9pt7b.h>


class SpotifyComponents: public Adafruit_ILI9341 {

  public:
    /**
     * SpotifyComponents constructor that call Adafruit_ILI9341 constructor
     * 
     * @param TFT_CS Display CS pin
     * @param TFT_DC Display DC pin
     * @param TFT_RST Display RST pin
     */
    SpotifyComponents(int8_t TFT_CS, int8_t TFT_DC, int8_t TFT_RST);

    /**
     * Print play button on the screen
     */
    void drawPlay();

    /**
     * Print pause button on the screen
     */
    void drawPause();

    /**
     * Print progress bar on the screen
     * 
     * @param value Value (from 0 to 100) that represent the progress bar
     */
    void drawProgressBar(int value);

    /**
     * Print skip arrows on the screen
     */
    void drawSkipArrows();

    /**
     * Print song title on the screen
     * 
     * @param t Characters to print on the screen as title
     */
    void drawTitle(const char* t);

    /**
     * Print the artist name on the screen
     * 
     * @param t Characters to print on the screen as artist name
     */
    void drawArtist(const char* a);

    /**
     * Print a text on the center on the screen
     * 
     * @param t Characters to print on the screen
     */
    void drawCenterText(const char* s);


  private:
    /**
     * Change characters that are not in ascii table to equivalent characters in ascii table
     * 
     * @param t Characters to sanitize
     */
    char* sanitizeString(const char* str);
  

    

};

