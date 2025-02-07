#include "Vector2f.h"

// constructor
Vector2f::Vector2f(float newx, float newy) : x(newx), y(newy){}

float Vector2f::operator()() const {
        return std::sqrt(x*x + y*y);
}


