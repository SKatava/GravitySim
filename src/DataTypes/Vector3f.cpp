#include "Vector3f.h"

// constructor
Vector3f::Vector3f(float newx, float newy, float newz) : x(newx), y(newy), z(newz){}

Vector3f Vector3f::operator+(const Vector3f& other) const{
    float x = (other.x + this->x)/2;
    float y = (other.y + this->y)/2;
    float z = (other.z + this->z)/2;
    return Vector3f(x, y, z);

}

