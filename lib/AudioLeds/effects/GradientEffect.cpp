#include "GradientEffect.h"

GradientEffect::GradientEffect()
{
    //put as many colors as you want
    colors = new std::vector<CRGB>();
    colors->push_back(CRGB::Blue);
    colors->push_back(CRGB::Orange);
    colors->push_back(CRGB::Red);
}

void GradientEffect::fill()
{
    //draw gradients
    uint16_t d = FastLED.size() / (colors->size() - 1);
    for (size_t i = 0; i < (colors->size() - 1); i++)
    {
        uint16_t startIndex = d * i;
        uint16_t lastIndex = startIndex + d;

        fill_gradient_RGB(FastLED.leds(), startIndex, colors->at(i), lastIndex, colors->at(i + 1));
    }

    //turn off remainder after fillValue
    fill_solid(&(FastLED.leds()[getFillValue()]), FastLED.size() - getFillValue(), CRGB::Black);

#ifdef DEBUG_EFFECT
    USE_SERIAL.println(getFillValue());
#endif
}