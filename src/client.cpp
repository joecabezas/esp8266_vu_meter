#ifdef CLIENT
// pinout documentation found at:
// https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/I2SInput/I2SInput.ino

#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

#include "AudioLeds.h"
#include "effects/RainbowEffect.h"

#define WEBSOCKETS_HOST "192.168.4.1"

#ifndef DEBUG
#undef DEBUG_NETWORK
#undef DEBUG_VOLUME
#endif

AudioLeds *audioLeds;
WebSocketsClient *webSocketsClient;

#define USE_SERIAL Serial

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{

    switch (type)
    {
    case WStype_DISCONNECTED:
#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("[WSc] Disconnected!\n");
#endif
        break;
    case WStype_CONNECTED:
#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
#endif
        break;
    case WStype_TEXT:
#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("[WSc] get text: %s\n", payload);
#endif
        break;
    case WStype_BIN:
#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
        hexdump(payload, length);
        for (size_t i = 0; i < length; i++)
        {
            USE_SERIAL.printf("payload[%d] %d\n", i, payload[i]);
        }
#endif
        {
            uint16_t volume = 0;
            for (size_t i = 0; i < length; i++)
            {
                volume += payload[i] << 8 * i;
            }
#ifdef DEBUG_VOLUME
            USE_SERIAL.println(volume);
#endif
            // audioLeds->loop(volume);
        }
        break;
    case WStype_PING:
#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("[WSc] get ping\n");
#endif
        break;
    case WStype_PONG:
#ifdef DEBUG_NETWORK
        USE_SERIAL.printf("[WSc] get pong\n");
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
#endif

#ifdef DEBUG_NETWORK
    USE_SERIAL.setDebugOutput(true);

    for (uint8_t t = 4; t > 0; t--)
    {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
#endif

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

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
#ifdef DEBUG_NETWORK
        USE_SERIAL.print('>');
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
        webSocketsClient = new WebSocketsClient();
        webSocketsClient->begin(WEBSOCKETS_HOST, WEBSOCKETS_PORT);
        webSocketsClient->onEvent(webSocketEvent);
        webSocketsClient->setReconnectInterval(2000);
    }

    audioLeds = new AudioLeds();

    RainbowEffect *rainbowEffect = new RainbowEffect();
    audioLeds->addEffect(rainbowEffect);
}

void loop()
{
    webSocketsClient->loop();
}
#endif