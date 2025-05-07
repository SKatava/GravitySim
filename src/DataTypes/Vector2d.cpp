#include "Vector2d.h"
#include <math.h>

// constructor
Vector2d::Vector2d(double newx, double newy) : x(newx), y(newy){}

// get lenght from vector
double Vector2d::operator()() const {
        return std::sqrt(x*x + y*y);
}

bool Vector2d::operator<(const Vector2d& other) const {
    if (x != other.x) return x < other.x;
    return y < other.y;
}
