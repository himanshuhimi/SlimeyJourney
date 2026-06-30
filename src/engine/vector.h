#pragma once

#include <cmath>

struct Vector2D
{
    float x, y;
    Vector2D(float x = 0.0f, float y = 0.0f);
    float distanceFromVec(Vector2D secVector);
    float getLength();
    Vector2D normalise();
    Vector2D operator+(const Vector2D &rhs);
    Vector2D operator-(const Vector2D &rhs);
};