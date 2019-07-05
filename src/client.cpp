#ifdef CLIENT
// pinout documentation found at:
// https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/I2SInput/I2SInput.ino

#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

#include "AudioLeds.h"
#include "effects/RainbowEffect.h"

//this is set as `build_flags`
// #define MICROPHONE_STATION_SSID "k014micstation"
// #define MICROPHONE_STATION_PASSWORD ""
// #define WEBSOCKETS_PORT 8080

#define WEBSOCKETS_HOST "192.168.4.1"

#define DEBUG

#ifdef DEBUG
// #define DEBUG_VOLUME
#define DEBUG_NETWORK
#endif

// AudioLeds *audioLeds;

WebSocketsClient webSocketsClient;

#define USE_SERIAL Serial

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{

    switch (type)
    {
    case WStype_DISCONNECTED:
        USE_SERIAL.printf("[WSc] Disconnected!\n");
        break;
    case WStype_CONNECTED:
        USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
        break;
    case WStype_TEXT:
        USE_SERIAL.printf("[WSc] get text: %s\n", payload);
        break;
    case WStype_BIN:
        USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
        hexdump(payload, length);
        // audioLeds->loop(payload[0]);
        break;
    case WStype_PING:
        USE_SERIAL.printf("[WSc] get ping\n");
        break;
    case WStype_PONG:
        USE_SERIAL.printf("[WSc] get pong\n");
        break;
    default:
        break;
    }
}

void setup()
{
#ifdef DEBUG
    USE_SERIAL.begin(9600);
    USE_SERIAL.setDebugOutput(true);
#endif

    for (uint8_t t = 4; t > 0; t--)
    {
#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
#endif
    }

    //The following line is fucking important and not documented anywhere
    //after a 3 year (and ongoing) issue, finally somebody came with this
    //solution
    //https://github.com/Links2004/arduinoWebSockets/issues/102
    WiFi.mode(WIFI_STA);
    
    WiFi.begin(MICROPHONE_STATION_SSID, MICROPHONE_STATION_PASSWORD);
#ifdef DEBUG_NETWORK
    USE_SERIAL.print("Connecting to ");
    USE_SERIAL.print(MICROPHONE_STATION_SSID);
    USE_SERIAL.println(" ...");
#endif

    int i = 0;
    while (WiFi.status() != WL_CONNECTED)
    { // Wait for the Wi-Fi to connect
        delay(500);
#ifdef DEBUG_NETWORK
        USE_SERIAL.print(++i);
        USE_SERIAL.print(' ');
#endif
    }

#ifdef DEBUG_NETWORK
    USE_SERIAL.println('\n');
    USE_SERIAL.println("Connection established!");
    USE_SERIAL.print("Connected to ");
    USE_SERIAL.println(WiFi.SSID());
    USE_SERIAL.print("IP address:\t");
    USE_SERIAL.println(WiFi.localIP());
    USE_SERIAL.print("WiFi status:\t");
    USE_SERIAL.println(WiFi.status());
#endif

    if (WiFi.status() == WL_CONNECTED)
    {
        // webSocketsClient = new WebSocketsClient();
        webSocketsClient.begin(WEBSOCKETS_HOST, WEBSOCKETS_PORT);
        // webSocketsClient.setAuthorization("user", "Password");
        webSocketsClient.onEvent(webSocketEvent);
        webSocketsClient.setReconnectInterval(2000);
    }
    // audioLeds = new AudioLeds();

    // RainbowEffect *rainbowEffect = new RainbowEffect();
    // audioLeds->addEffect(rainbowEffect);
}

void loop()
{
    webSocketsClient.loop();
}
#endif