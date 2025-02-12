#ifndef VECTOR3F_CLASS_H
#define VECTOR3F_CLASS_H


class Vector3f
{
    public:
    Vector3f(float newx, float newy, float newz);
    Vector3f operator+(const Vector3f& other) const;
    float x;
    float y;
    float z;
};

#endif