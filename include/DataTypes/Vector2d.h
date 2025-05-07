#ifndef VECTOR2D_CLASS_H
#define VECTOR2D_CLASS_H

class Vector2d{
    public:
        Vector2d(double newx, double newy);
        double x;
        double y;
        double operator()() const;
        bool operator<(const Vector2d& other) const;
    private:
};

#endif
