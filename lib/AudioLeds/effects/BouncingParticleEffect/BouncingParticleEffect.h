#ifndef BOUNCING_PARTICLE_EFFECT_H
#define BOUNCING_PARTICLE_EFFECT_H

#include "effects/BaseEffect.h"

#include <vector>

class BouncingParticle
{
private:
    bool dying;

    float position;
    float velocity;
    float acceleration;
    float size;

    CRGB color;

public:
    bool dead;
    BouncingParticle();
    void tick();
};

class BouncingParticleEffect : public BaseEffect
{
private:
    std::vector<BouncingParticle *> *particles;
    uint32_t millisecondsPassed;
    uint32_t millisecondsSinceLastParticle;

    void createParticle();
    void tick();

protected:
    virtual void loop() override;
    virtual void fill() override;

public:
    BouncingParticleEffect();
};

#endif