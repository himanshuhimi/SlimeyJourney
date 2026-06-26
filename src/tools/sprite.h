#pragma once

#include "core/config.h"

class Sprite
{
public:
    SDL_Renderer *renderer = nullptr;
    Vector2D Original, Camera, Velocity, Position, Center;
    LineOfSight gravityLOS, lineOfSight;
    SDL_FRect hitbox, rect, dst;
    Image image;
    bool movable = true;
    float left, right, bottom, top;
    enum Direction
    {
        Left,
        Right
    } lastDirection;
    struct SpriteStates
    {
        bool prevOnGround, onGround, inAir, walking;
    } states;
    Sprite(SDL_Renderer *renderer, string imgSource, float x, float y);
    void handle(double dt, const vector<Object> &objects);
    void render(Vector2D Camera);
    void handleMovement(double dt);
    void handleLOS();
    void handleGravity(double dt, const vector<Object> &objects);
    void handleStates(bool onGround, bool prevOnGround);
};