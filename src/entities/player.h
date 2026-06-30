#pragma once

#include "tools/sprite.h"
#include "ball.h"
#include "ui/progress.h"
#include "tools/enemy.h"

class Player : public Sprite
{
public:
    Vector2D prevPos;
    Cooldown throwCooldown = {1.0};
    Enemy *combatEnemy = nullptr;
    map<string, Animation> anims;
    map<string, Audio> audios;
    vector<Ball> balls = {};
    int maxHP = 5, HP = maxHP;
    float speed, jumpStrength;
    bool inCombat = false, dead = false, immune = false, mouseClicked = false;
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Object> &objects);
    void render(Vector2D Camera);
    void damage(int byPoints = 1);
    void attack();
    void resetPos(bool previous = true);
    void handleMovement(double dt);
    void handleShooting(double dt);
};