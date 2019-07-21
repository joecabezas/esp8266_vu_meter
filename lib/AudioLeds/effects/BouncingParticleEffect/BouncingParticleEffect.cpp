#include "BouncingParticleEffect.h"

#define BOUNCING_PARTICLE_FRAME_DELAY 5
#define NEW_PARTICLE_MINIMUM_DELAY 20

BouncingParticle::BouncingParticle()
{
    dying = dead = false;
    color = CHSV(random8(), 255, 255);
    position = NUMBER_OF_LEDS - 1;
    size = 4.0f;
    velocity = 0.0f;
    acceleration = -0.05f;
}

void BouncingParticle::tick()
{
    if (dying)
    {
        color.fadeToBlackBy(60);
        dead = !color;
    }

    for (size_t i = 0; i < size; i++)
    {
        FastLED[0][position + i] = color;
    }

    position = _max(0.0f, position + velocity);
    velocity += acceleration;

    if (position == 0.0f)
        velocity *= -0.60;

    //check if particle needs to die
    dying = (position * position < 0.40f && velocity * velocity < 0.40f);

    // #ifdef DEBUG_EFFECT
    //     USE_SERIAL.printf(
    //         "%f, %f, %d, %d\n",
    //         position,
    //         velocity,
    //         (std::abs(position) < 0.4f && std::abs(velocity) < 0.4f) ? 2 : 0,
    //         (color.r == 0 && color.g == 0 && color.b == 0) ? 3 : 0);
    // #endif
}

BouncingParticleEffect::BouncingParticleEffect()
{
    particles = new std::deque<BouncingParticle *>();

    millisecondsPassed = millis();
    millisecondsSinceLastParticle = millis();
}

void BouncingParticleEffect::createParticle()
{
    particles->push_back(new BouncingParticle());
}

void BouncingParticleEffect::tick()
{
    if (particles->size() == 0)
        return;

#ifdef DEBUG_EFFECT
    USE_SERIAL.printf(">size: %d\n", particles->size());
#endif

    std::deque<BouncingParticle *>::iterator it = particles->begin();

    while (true)
    {
        if ((*it)->dead)
        {
            delete *it;
            it = particles->erase(it);

            if (it == particles->end())
                break;

            continue;
        }

        (*it)->tick();

        if (++it == particles->end())
            break;
    }
}

void BouncingParticleEffect::fill()
{
    if (millis() - millisecondsPassed > BOUNCING_PARTICLE_FRAME_DELAY)
    {
        millisecondsPassed = millis();
        clean();
        tick();

        if (getFillValue() > NUMBER_OF_LEDS * 0.80f)
        {
            input = 0.0f;
            createParticle();
        }
    }

    // clean();
    // tick();

    // if (getFillValue() > NUMBER_OF_LEDS * 0.80f)
    // {
    //     input = 0.0f;
    //     createParticle();
    // }

    // if (
    //     millis() - millisecondsSinceLastParticle > NEW_PARTICLE_MINIMUM_DELAY &&
    //     getFillValue() > NUMBER_OF_LEDS * 0.50f)
    // {
    //     millisecondsSinceLastParticle = millis();
    //     // createParticle();
    // }
}

void BouncingParticleEffect::loop()
{
    updateInput();
    fill();
}