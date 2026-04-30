#pragma once

#include "../tools/sprite.h"
#include "grass.h"

class Player : public Sprite
{
public:
    vector<Grass *> colGrasses;
    int speed;
    float jumpStrength;
    bool onGround;
    struct PlayerStates
    {
        bool jumping, walking;
    } state;
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Grass *> &grasses);
};