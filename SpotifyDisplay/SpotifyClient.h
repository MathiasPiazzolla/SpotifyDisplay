#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <base64.h>



class SpotifyClient {

  public:
    /**
     * SpotifyClient constructor
     * 
     * @param access_token Spotify access token
     * @param refresh_token Spotify refresh token
     * @param client_id Spotify client id
     * @param client_secret Spotify client secret
     */
    SpotifyClient(String access_token, String refresh_token, String client_id, String client_secret);

    /**
     * Make a request to skit actual trak
     * 
     * @param req The type of request. if req == "left": skip to previous track else if req == "right": skip to next track else error
     * @return Int that represent the status (-1: wifi disconnected, 0: error, 1: success)
     */
    int spotifySkipRequest(char* req);

    /**
     * Make a request to play or pause actual track
     * 
     * @param req The type of request. if req == "play": play actual track else if req == "pause": pause actual track else error
     * @return Int that represent the status (-1: wifi disconnected, 0: error, 1: success)
     */
    int spotifyPlayPauseRequest(char* req);

    /**
     * Make a request to get actual playback state and update out array with this informations
     * 
     * @param out Array that in the end contains actual playback state in this position: 0: title, 1: artist, 2: imageURL, 3: play, 4: progress, 5: duration
     * @return Int that represent the status (-1: wifi disconnected, 0: error, 1: success)
     */
    int getSpotifyInformations(String out[]);


  private:
    /* Spotify access token */
    String access_token;
    /* Spotify refresh token */
    String refresh_token; 
    /* Spotify client id */
    String client_id;
    /* Spotify client secret */
    String client_secret;

    /**
     * Function that make a request to ask new access token (usually when the previous access token was expired)
     * 
     * @return Int that represent the status (-1: wifi disconnected, 0: error, 1: success)
     */
    int refreshToken();
};

