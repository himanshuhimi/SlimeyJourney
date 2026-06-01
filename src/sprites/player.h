#pragma once

#include "../tools/sprite.h"
#include "grass.h"
#include "ball.h"

class Player : public Sprite
{
public:
    Cooldown throwCooldown = {1.0};
    std::unordered_map<string, Animation *> anims;
    std::unordered_map<string, Audio *> audios;
    enum Direction {Left, Right} lastDirection;
    vector<Ball> balls;
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
};