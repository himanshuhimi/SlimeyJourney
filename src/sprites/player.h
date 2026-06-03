#pragma once

#include "../tools/sprite.h"
#include "grass.h"
#include "ball.h"

class Player : public Sprite
{
public:
    unordered_map<string, Animation *> anims;
    unordered_map<string, Audio *> audios;
    Vector2D prevPos;
    Cooldown throwCooldown = {1.0};
    vector<Ball> balls = {};
    bool mouseClicked = false;
    int HP = 3;
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
    void handleJump(double dt, const bool *keys);
    void handleShooting(double dt);
};