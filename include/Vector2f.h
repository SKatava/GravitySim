#ifndef VECTOR2F_CLASS_H
#define VECTOR2F_CLASS_H

#include <math.h>

class Vector2f
{
    public:
    Vector2f(float newx, float newy);
    float x;
    float y;
    float operator()() const;
};

#endif