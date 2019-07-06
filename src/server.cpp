#ifdef SERVER
// pinout documentation found at:
// https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/I2SInput/I2SInput.ino

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

#include "Microphone.h"

#ifndef DEBUG
#undef DEBUG_NETWORK
#endif

Microphone *microphone;
WebSocketsServer *webSocketsServer;

#define USE_SERIAL Serial

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("[%u] Disconnected!\n", num);
#endif
        break;
    case WStype_CONNECTED:
    {
#ifdef DEBUG_NETWORK
        IPAddress ip = webSocketsServer->remoteIP(num);
        USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
#endif
    }
    break;
    case WStype_TEXT:
#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);
#endif
        break;
    case WStype_BIN:
#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("[%u] get binary length: %u\n", num, length);
        hexdump(payload, length);
#endif
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

#ifdef DEBUG_NETWORK
    for (uint8_t t = 4; t > 0; t--)
    {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
#endif

    WiFi.softAP(MICROPHONE_STATION_SSID, MICROPHONE_STATION_PASSWORD);
#ifdef DEBUG_NETWORK
    USE_SERIAL.print("AP IP:\t");
    USE_SERIAL.println(WiFi.softAPIP());
#endif

    webSocketsServer = new WebSocketsServer(WEBSOCKETS_PORT);
    webSocketsServer->begin();
    webSocketsServer->onEvent(webSocketEvent);

    microphone = new Microphone(16000);
}

void loop()
{
    webSocketsServer->loop();

    int volume = microphone->getVolume();
    webSocketsServer->broadcastBIN((uint8_t *)&volume, sizeof(volume));
}
#endif