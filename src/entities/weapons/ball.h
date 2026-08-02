#pragma once

#include "tools/sprites/weapon.h"

class Ball : public Weapon
{
public:
    Ball(SDL_Renderer *renderer, float x, float y, string type, Vector2D Direction);
};