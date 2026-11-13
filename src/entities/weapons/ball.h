#pragma once

#include "tools/sprites/weapon.h"

struct Ball : public Weapon
{

    Ball(SDL_Renderer *renderer, float x, float y, string type, Vector2D Direction);
};