#include "Vector2f.h"

// constructor
Vector2f::Vector2f(float newx, float newy) : x(newx), y(newy){}

// get lenght from vector
float Vector2f::operator()() const {
        return std::sqrt(x*x + y*y);
}

bool Vector2f::operator<(const Vector2f& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
}

Vector2f Vector2f::operator+(const Vector2f& other) const {
        return Vector2f(this->x + other.x, this->y + other.y);
}

Vector2f Vector2f::operator+=(const Vector2f& other){
        x += other.x;
        y += other.y;
        return *this;
}
