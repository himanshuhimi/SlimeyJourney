#pragma once

#include "tools/sprite.h"
#include "ball.h"
#include "ui/progress.h"

class Player : public Sprite
{
public:
    Vector2D prevPos;
    Cooldown throwCooldown = {1.0};
    bool inCombat = false, dead = false, immune = false, mouseClicked = false;
    map<string, Animation> anims;
    map<string, Audio> audios;
    vector<Ball> balls = {};
    int maxHP = 5, HP = maxHP;
    float speed, jumpStrength;
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Object> &grasses);
    void render(Vector2D Camera);
    void damage(Progress healthBar, int byPoints = 1);
    void resetPos(Progress healthBar, bool previous = true);
    void handleMovement(double dt);
    void handleShooting(double dt);
};