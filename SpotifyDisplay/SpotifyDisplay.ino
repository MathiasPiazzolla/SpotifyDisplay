#include "SpotifyComponents.h"
#include "SpotifyClient.h"
#include <HTTPClient.h>
#include "JPEGDEC.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <XPT2046_Touchscreen.h>

#include "secrets.h"

#define TIMEOUT_IMAGE 4000

// display calibration variable
#define TS_MINX 3900  // to change
#define TS_MINY 3900  // to change
#define TS_MAXX 220   // to change 
#define TS_MAXY 300   // to change

// display pins
#define TFT_CS 5
#define TFT_DC 4
#define TFT_RST 2
#define T_CS  22

#define ROTATION 0

/* Spotify access token */
String access_token = ACCESS_TOKEN;
/* Spotify refresh token */
String refresh_token = REFRESH_TOKEN;

SpotifyComponents tft = SpotifyComponents(TFT_CS, TFT_DC, TFT_RST);
SpotifyClient sc = SpotifyClient(ACCESS_TOKEN, REFRESH_TOKEN, CLIENT_ID, CLIENT_SECRET);
XPT2046_Touchscreen ts(T_CS);

String title = "";
String artist = "";
String imageURL = "";
int progress = 1;
int duration = 1;
bool play = false;

TaskHandle_t TouchTask; // display touch task


void setup() {
  Serial.begin(115200);

  // Display initializzazion
  tft.begin();
  tft.setRotation(ROTATION);
  tft.fillScreen(ILI9341_BLACK);

  // touchscreen initializzation
  ts.begin();
  ts.setRotation(ROTATION);


  // WiFI
  WiFiMulti wifiMulti;
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  wifiMulti.addAP(WIFI_SSID_2, WIFI_PASSWORD_2);

  while (wifiMulti.run() != WL_CONNECTED) {
    tft.drawCenterText("Connecting...");
    Serial.println("Connecting");
  }
  // wifi
  // while (WiFi.status() != WL_CONNECTED) {
  //   tft.drawCenterText("Try WiFi 1");
  //   delay(1000);
  //   tft.drawCenterText("Connecting...");
  //   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  //   Serial.println("Connecting");
  //   int i = 0;
  //   while (WiFi.status() != WL_CONNECTED && i != 30) {
  //     delay(500);
  //     Serial.print(".");
  //     i++;
  //   }
  //   if (WiFi.status() != WL_CONNECTED) {
  //     tft.drawCenterText("Try WiFi 2");
  //     delay(1000);
  //     tft.drawCenterText("Connecting...");
  //     WiFi.begin(WIFI_SSID_2, WIFI_PASSWORD_2);
  //     Serial.println("Connecting");
  //     int i = 0;
  //     while (WiFi.status() != WL_CONNECTED && i != 30) {
  //       delay(500);
  //       Serial.print(".");
  //       i++;
  //     }
  //   }
  // }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  tft.drawCenterText("Connected :)");

  // task per la gestione del touch
  xTaskCreatePinnedToCore(
                    TouchTaskCode,   /* Task function. */
                    "TouchTask",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &TouchTask,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */    
}

/* Manage if I already print something */
bool done = false;
void loop() {

  String out[6];
  int code = sc.getSpotifyInformations(out);
  checkRequest(code);


  if (code == 0) { // if there is no song
    if (!done) {
      tft.fillScreen(ILI9341_BLACK);
      tft.drawCenterText("No songs playing!");
      title = "";
      artist = "";
      imageURL = "";
      progress = 1;
      duration = 1;
      play = false;
      done = true;
    }
  } else {
    int percentuale = 0;
    done = false;
    progress = out[4].toInt();
    duration = out[5].toInt();

    if (((out[3] == "1" || out[3] == "true") && play == false) || ((out[3] == "0" || out[3] == "false") && play == true)) {
      // update play variable
      if (out[3] == "0" || out[3] == "false") {
        play = false;
      } else {
        play = true;
      }

      // update play/pause button
      if (play) {
        tft.drawPlay();
      } else {
        tft.drawPause();
      }
    }

    // update progress bar
    if (play) {
      progress += 100;
      if (progress > duration) {
        progress = duration;
      }
    }

    if (progress != 1 && duration != 1) {
      percentuale = (progress * 100) / duration;
      tft.drawProgressBar(percentuale);
    }

    // update title, artist and image
    if (out[0] != title || out[1] != artist || out[2] != imageURL) {
      title = out[0];
      artist = out[1];
      imageURL = out[2];
      tft.fillScreen(ILI9341_BLACK);
      tft.drawSkipArrows();
      tft.drawArtist(artist.c_str());
      tft.drawTitle(title.c_str());
      tft.drawPlay();
      tft.drawProgressBar(percentuale);
      downloadAndDisplayImage();
    }

  }

 delay(100); // to avoid that esp32's watchdog reset itself

}

/**
 * Task function to check if the display was touched
 *
 * @param pvParameters Task parameters
 */
void TouchTaskCode(void* pvParameters){
  for(;;){
    touch_control();
    delay(10); // to avoid that esp32's watchdog reset itself
  } 
}

/**
 * Manage the result of SpotifyClient requests
 *
 * @param res SpotifyRequest return variable
 */
void checkRequest(int res) {
  if (res == -1) {
    tft.drawCenterText("WiFi Disconnected");
    delay(5000);
    ESP.restart();
  }
}

/* Variable to manage if i press and release the screen or not */
bool wastouched = false;
/**
 * Check if someone touch the screen and make the right action
 */
void touch_control(){
  if (ts.touched()){
    TS_Point p = ts.getPoint();
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

    // left skip
    if (p.x >= 29 && p.x <= 65 && p.y >= tft.height()-40-20 && p.y <= tft.height()-40+20 && !wastouched) {
      wastouched = true;
      Serial.println("Left skip");
      checkRequest(sc.spotifySkipRequest("left"));
    }

    // right skip
    if (p.x >= tft.width()-65 && p.x <= tft.width()-34+6 && p.y >= tft.height()-40-20 && p.y <= tft.height()-40+20 && !wastouched) {
      wastouched = true;
      Serial.println("Right skip");
      checkRequest(sc.spotifySkipRequest("right"));
    }

    // play/pause
    if (p.x >= tft.width()/2-26 && p.x <= tft.width()/2+26 && p.y >= tft.height()-40-26 && p.y <= tft.height()-40+26 && !wastouched) {
      wastouched = true;
      Serial.println("Play/pause");
      if (play) {
        checkRequest(sc.spotifyPlayPauseRequest("pause"));
      } else {
        checkRequest(sc.spotifyPlayPauseRequest("play"));
      }
    }
  } else {
    wastouched = false;
  }
}


JPEGDEC jpeg;
/**
 * Function to print the downloaded image
 *
 * @param pDraw image data structure
 * @return 1 if everything is done
 */
int JPEGDraw(JPEGDRAW* pDraw) {
  tft.dmaWait();
  tft.setAddrWindow(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
  tft.writePixels(pDraw->pPixels, pDraw->iWidth * pDraw->iHeight, true, false);
  return 1;
}

/**
 * Function that make a request to download an image and print that image on tft screen.
 */
void downloadAndDisplayImage() {
  Serial.println("Display image");

  HTTPClient http;
  http.begin(imageURL);
  http.setTimeout(TIMEOUT_IMAGE);

  int httpCode = http.GET();
  Serial.print("Resp code image: ");
  Serial.println(httpCode);
  if (httpCode == HTTP_CODE_OK) {

    String s = http.getString();

    const char* data = s.c_str();

    tft.startWrite();
    if (jpeg.openFLASH((uint8_t*)data, s.length(), JPEGDraw)) {
      if (!jpeg.decode((tft.width() - 150) / 2, 20, JPEG_SCALE_HALF)) {
        Serial.println("Error decode image");
      }
      jpeg.close();
    } else {
      Serial.print("length: ");
      Serial.println(s.length());
      Serial.println("Error decode image");
    }
    tft.endWrite();
  } else {
    imageURL = ""; //imageURL reset to refresh it
    Serial.printf("Errore HTTP: %d\n", httpCode);
  }
  http.end();
  Serial.println("End display image");
}
