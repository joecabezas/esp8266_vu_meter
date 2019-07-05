#ifdef SERVER
// pinout documentation found at:
// https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/I2SInput/I2SInput.ino

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

// #include "Microphone.h"

//this is set as `build_flags`
// #define MICROPHONE_STATION_SSID "k014micstation"
// #define MICROPHONE_STATION_PASSWORD ""
// #define WEBSOCKETS_PORT 8080

#define DEBUG

#ifdef DEBUG
#define DEBUG_NETWORK
#endif

// Microphone *microphone;
WebSocketsServer webSocketsServer = WebSocketsServer(WEBSOCKETS_PORT);

#define USE_SERIAL Serial

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        USE_SERIAL.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = webSocketsServer.remoteIP(num);
        USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        webSocketsServer.sendTXT(num, "Connected");
    }
    break;
    case WStype_TEXT:
        USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);
        break;
    case WStype_BIN:
        USE_SERIAL.printf("[%u] get binary length: %u\n", num, length);
        hexdump(payload, length);
        break;
        // default:
        //     break;
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

    WiFi.softAP(MICROPHONE_STATION_SSID, MICROPHONE_STATION_PASSWORD);
#ifdef DEBUG_NETWORK
    USE_SERIAL.print("AP IP:\t");
    USE_SERIAL.println(WiFi.softAPIP());
#endif

    // webSocketsServer = new WebSocketsServer(WEBSOCKETS_PORT);
    webSocketsServer.begin();
    webSocketsServer.onEvent(webSocketEvent);

    // microphone = new Microphone(16000);
}

void loop()
{
    webSocketsServer.loop();
    // int volume = microphone->getVolume();
    // webSocketsServer.broadcastBIN((uint8_t *)&volume, sizeof(volume));
}
#endif