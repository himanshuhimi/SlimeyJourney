#pragma once

#include "../tools/sprite.h"
#include "grass.h"
#include "ball.h"

class Player : public Sprite
{
public:
    int HP = 3;
    bool mouseClicked = false;
    Vector2D prevPos;
    Cooldown throwCooldown = {1.0};
    std::unordered_map<string, Animation *> anims;
    std::unordered_map<string, Audio *> audios;
    vector<Ball> balls;
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
    void handleJump(double dt);
    void handleShooting(double dt);
};