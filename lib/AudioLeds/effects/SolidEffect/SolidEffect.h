#ifndef SOLID_EFFECT_H
#define SOLID_EFFECT_H

#include "effects/BaseVUEffect.h"

class SolidEffect: public BaseVUEffect
{
    public:
        virtual void clean() override;
        virtual void fill() override;
};

#endif