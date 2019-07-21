#include "BouncingParticleEffect.h"

#define BOUNCING_PARTICLE_FRAME_DELAY 2
#define BOUNCING_PARTICLE_FILL_VALUE_FACTOR_ACTIVATOR 0.90f

BouncingParticle::BouncingParticle()
{
    dying = dead = false;
    color = CHSV(random8(), 255, 255);
    position = NUMBER_OF_LEDS - 1;
    size = 4.0f;
    velocity = 0.0f;
    acceleration = -0.06f;
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
        size_t index = position + i;

        if (index >= NUMBER_OF_LEDS)
            continue;

        FastLED[0][index] = color;
    }

    position = _max(0.0f, position + velocity);
    velocity += acceleration;

    if (position == 0.0f)
        velocity *= -0.60;

    //check if particle needs to die
    dying = (position * position < 0.40f && velocity * velocity < 0.40f);
}

BouncingParticleEffect::BouncingParticleEffect()
{
    particles = new std::deque<BouncingParticle *>();

    millisecondsPassed = millis();
    millisecondsSinceLastParticle = millis();
}

void BouncingParticleEffect::sleep()
{
    particles->clear();
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

        if (getFillValue() > NUMBER_OF_LEDS * BOUNCING_PARTICLE_FILL_VALUE_FACTOR_ACTIVATOR)
        {
            input = 0.0f;
            createParticle();
        }
    }
}

void BouncingParticleEffect::loop()
{
    updateInput();
    fill();
}