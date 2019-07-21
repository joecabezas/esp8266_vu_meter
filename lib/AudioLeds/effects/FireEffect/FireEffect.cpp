#include "FireEffect.h"

#define FIRE_EFFECT_FRAME_DELAY 30

// FireEffect, code taken and adapted from latest version of FastLED examples:
// https://github.com/FastLED/FastLED/blob/962b1205a7824ff79e2e585f221759accb2ecfd9/examples/Fire2012WithPalette/Fire2012WithPalette.ino

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
////
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation,
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUMBER_OF_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking.
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100
#define COOLING 60

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 50

FireEffect::FireEffect() : FireEffect(HeatColors_p) {}

FireEffect::FireEffect(CRGBPalette16 _pallete)
{
    // pallete = _pallete;
    // This first palette is the basic 'black body radiation' colors,
    // which run from black to red to bright yellow to white.
    pallete = HeatColors_p;

    // These are other ways to set up the color palette for the 'fire'.
    // First, a gradient from black to red to yellow to white -- similar to HeatColors_p
    //   pallete = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);

    // Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
    //   pallete = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);

    // Third, here's a simpler, three-step gradient, from black to red to white
    //   pallete = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);

    millisecondsPassed = millis();
}

void FireEffect::setInputValue(uint8_t inputValue)
{
}

void FireEffect::Fire2012WithPalette()
{
    // Array of temperature readings at each simulation cell
    static byte heat[NUMBER_OF_LEDS];

    // Step 1.  Cool down every cell a little
    for (int i = 0; i < NUMBER_OF_LEDS; i++)
    {
        heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUMBER_OF_LEDS) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = NUMBER_OF_LEDS - 1; k >= 2; k--)
    {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < SPARKING)
    {
        int y = random8(7);
        heat[y] = qadd8(heat[y], random8(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (int j = 0; j < NUMBER_OF_LEDS; j++)
    {
        // Scale the heat value from 0-255 down to 0-240
        // for best results with color palettes.
        byte colorindex = scale8(heat[j], 240);
        CRGB color = ColorFromPalette(pallete, colorindex);
        int pixelnumber;
        if (gReverseDirection)
        {
            pixelnumber = (NUMBER_OF_LEDS - 1) - j;
        }
        else
        {
            pixelnumber = j;
        }
        FastLED.leds()[pixelnumber] = color;
    }
}

void FireEffect::loop()
{
    if (millis() - millisecondsPassed > FIRE_EFFECT_FRAME_DELAY)
    {
        millisecondsPassed = millis();

        // Add entropy to random number generator; we use a lot of it.
        random16_add_entropy(random(0, 100));

        Fire2012WithPalette();
    }
}