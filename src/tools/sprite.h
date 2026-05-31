#pragma once

#include "../core/config.h"
#include "../sprites/grass.h"

class Sprite
{
public:
    SDL_Renderer *renderer = nullptr;
    Image image;
    Vector2D Velocity, Position;
    SDL_FRect dst, rect;
    bool isGravitational = false;
    float speed, jumpStrength;
    struct SpriteStates
    {
        bool onGround = false;
        bool jumping = false;
        bool walking = false;
    } state;
    Sprite(SDL_Renderer *renderer, 
        string imgSource, 
        float x, 
        float y, 
        bool isGravitational = false
    );
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
};