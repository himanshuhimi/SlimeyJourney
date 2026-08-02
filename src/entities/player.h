#pragma once

#include "tools/sprite.h"
#include "weapons/ball.h"
#include "tools/sprites/enemy.h"

class Player : public Sprite
{
public:
    Vector2D prevJumpPos;
    vector<Ball> balls = {};
    Enemy *combatEnemy = nullptr;
    map<string, Audio> audios = {};
    bool inCombat = false, dead = false, immune = false,
         mouseClicked = false, buffed = false;
    int maxHP = 5, HP = maxHP, enemiesKilled = 0;
    float baseSpeed = 32.0f * 5, speed = baseSpeed, 
        baseJumpStrength = 32.0f * 5, jumpStrength = baseJumpStrength;
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Object> &objects) override;
    void render(Vector2D Camera) override;
    void damage(int byPoints = 1);
    void attack();
    void resetPos(bool previous = true);

private:
    Cooldown throwCooldown = {1.0};
    map<string, Animation> anims = {};
    void handleMovement(double dt);
    void handleAttacking(double dt);
};