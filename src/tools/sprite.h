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
    virtual void handle(double dt, const vector<Object> &objects);
    virtual void render(Vector2D Camera);

protected:
    void moveX(double dt, const vector<Object> &objects);
    void moveY(double dt, const vector<Object> &objects);
    void handleMovement(double dt, const vector<Object> &objects);
    void handleLOS();
    void handleGravity(double dt);
};