#include "SpotifyClient.h"

#define TIMEOUT 2000


SpotifyClient::SpotifyClient(String access_token, String refresh_token, String client_id, String client_secret){
  this->access_token = access_token;
  this->refresh_token = refresh_token;
  this->client_id = client_id;
  this->client_secret = client_secret;
}


int SpotifyClient::spotifySkipRequest(char* req) {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = "";
    if (strcmp(req, "left") == 0) {
      serverPath = "https://api.spotify.com/v1/me/player/previous";
    } else if (strcmp(req, "right") == 0) {
      serverPath = "https://api.spotify.com/v1/me/player/next";
    } else {
      Serial.println("spotifySkipRequest: No valid parameter");
      return 0;
    }

    // prepare http request
    http.begin(serverPath.c_str());
    http.setTimeout(TIMEOUT);
    http.addHeader("Authorization", "Bearer " + this->access_token);
    http.addHeader("Content-Type", "application/json"); 
    http.addHeader("Content-Length", "0"); 

    // send http request
    int httpResponseCode = http.POST("");

    // check response code
    if (httpResponseCode <= 299) {
      Serial.print("HTTP Response code skip request: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code skip: ");
      Serial.println(httpResponseCode);
      http.end();
      return 0;
    }
  http.end();
  } else {
    Serial.println("WiFi Disconnected");
    return -1;
  }
  return 1;
}



int SpotifyClient::spotifyPlayPauseRequest(char* req) {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = "";
    if (strcmp(req, "play") == 0) {
      serverPath = "https://api.spotify.com/v1/me/player/play";
    } else if (strcmp(req, "pause") == 0) {
      serverPath = "https://api.spotify.com/v1/me/player/pause";
    } else {
      Serial.println("spotifyPlayPauseRequest: No valid parameter");
      return 0;
    }

    // prepare http request
    http.begin(serverPath.c_str());
    http.setTimeout(TIMEOUT);
    http.addHeader("Authorization", "Bearer " + this->access_token);
    http.addHeader("Content-Type", "application/json"); 
    http.addHeader("Content-Length", "0"); 

    // send http request
    int httpResponseCode = http.PUT("");

    // chech response code
    if (httpResponseCode <= 299) {
      Serial.print("HTTP Response code play/pause request: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code play/pause: ");
      Serial.println(httpResponseCode);
      http.end();
      return 0;
    }
  http.end();
  } else {
    Serial.println("WiFi Disconnected");
    return -1;
  }
  return 1;
}


int SpotifyClient::getSpotifyInformations(String out[]) {
  out[0] = "";
  out[1] = "";
  out[2] = "";
  out[3] = "false";
  out[4] = "1";
  out[5] = "1";

  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = "https://api.spotify.com/v1/me/player/currently-playing";

    // prepare http request
    http.begin(serverPath.c_str());
    http.setTimeout(TIMEOUT);
    http.addHeader("Authorization", "Bearer " + this->access_token);

    // send http request
    int httpResponseCode = http.GET();

    // check response code
    if (httpResponseCode == 204) {
      http.end();
      Serial.println("No content");
      return 0;
    }
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();

      JSONVar myObject = JSON.parse(payload);
      if (myObject.hasOwnProperty("error")) {
        Serial.println("Refresh token");
        http.end();
        return this->refreshToken();
      } else {

        out[4] = (int)myObject["progress_ms"];
        out[5] = (int)myObject["item"]["duration_ms"];

        out[3] = (bool)myObject["is_playing"];

        out[0] = (const char*)myObject["item"]["name"];
        
        out[2] = (const char*)myObject["item"]["album"]["images"][1]["url"];


        JSONVar artists = myObject["item"]["artists"];
        String strArtists;
        for (int i = 0; i < artists.length(); i++) {
          strArtists.concat((const char*)artists[i]["name"]);
          if (i != artists.length() - 1) {
            strArtists += ", ";
          }
        }
        out[1] = strArtists;
      }
    } else {
      Serial.print("Error code informations: ");
      Serial.println(httpResponseCode);
      http.end();
      return 1; // in caso di errore fa niente
    }
  http.end();
  } else {
    Serial.println("WiFi Disconnected");
    return -1;
  }
  return 1;
}


int SpotifyClient::refreshToken() {
  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String serverPath = "https://accounts.spotify.com/api/token";

    String cred = this->client_id;
    cred.concat(":");
    cred.concat(this->client_secret);

    String b64credentials = base64::encode(cred);
    b64credentials = "Basic " + b64credentials;

    // prepare http request
    http.begin(serverPath.c_str());
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Authorization", b64credentials);

    // send http request
    String httpRequestData = "grant_type=refresh_token&refresh_token=" + this->refresh_token;
    int httpResponseCode = http.POST(httpRequestData);

    // check response code
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      JSONVar myObject = JSON.parse(http.getString());

      if (myObject.hasOwnProperty("error")) {
        Serial.println("Errore refresh token");
        http.end();
        return 0;
      } else {
        access_token = (const char*)myObject["access_token"];
        this->access_token = access_token;
      }
    } else {
      Serial.print("Error code refresh: ");
      Serial.println(httpResponseCode);
      http.end();
      return 0;
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
    return -1;
  }
  return 1;
}

