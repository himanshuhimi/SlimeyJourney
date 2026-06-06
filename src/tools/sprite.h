#pragma once

#include "core/config.h"
#include "sprites/object.h"
#include "LOS.h"

class Sprite
{
public:
    SDL_Renderer *renderer = nullptr;
    Vector2D Original, Camera, Velocity, Position, Center;
    LOS gravityLOS, lineOfSight;
    SDL_FRect dst, rect;
    Image image;
    bool movable = true;
    enum Direction
    {
        Left,
        Right
    } lastDirection;
    struct SpriteStates
    {
        bool prevOnGround, onGround, jumping, walking;
    } state;
    Sprite(SDL_Renderer *renderer, string imgSource, float x, float y);
    void handle(double dt, const vector<Object> &grasses);
    void render(Vector2D Camera);
    void handleMovement(double dt);
    void handleLOS();
    void handleGravity(double dt, const vector<Object> &grasses);
    void handleStates(bool onGround, bool prevOnGround);
};