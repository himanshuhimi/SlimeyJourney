#pragma once

#include "../tools/sprite.h"
#include "grass.h"

class Player : public Sprite
{
public:
    int speed;
    float jumpStrength;
    bool spacePressed = false;
    std::unordered_map<string, Animation *> anims;
    enum Direction {Left, Right} lastDirection;
    struct PlayerStates
    {
        bool jumping, walking;
    } state;
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
};