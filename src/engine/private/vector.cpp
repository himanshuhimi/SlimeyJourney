#include "../vector.h"

Vector2D::Vector2D(float x, float y) : x(x), y(y) {};

float Vector2D::distanceFromVec(Vector2D secVector)
{
    return sqrt(pow(secVector.x - x, 2) + pow(secVector.y - y, 2));
}

float Vector2D::getLength() { return sqrt(pow(x, 2) + pow(y, 2)); }

Vector2D Vector2D::normalise()
{
    float length = getLength();
    if (length == 0.0)
        return Vector2D{0, 0};
    x /= length;
    y /= length;
    return Vector2D{x, y};
}

Vector2D Vector2D::operator+(const Vector2D &rhs) {return Vector2D(x + rhs.x, y +  rhs.y); }

Vector2D Vector2D::operator-(const Vector2D &rhs) {return Vector2D(x - rhs.x, y - rhs.y); }