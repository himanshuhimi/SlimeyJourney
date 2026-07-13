#pragma once

#include "tools/sprite.h"
#include "ball.h"
#include "tools/enemy.h"

class Player : public Sprite
{
public:
    Vector2D prevPos;
    vector<Ball> balls = {};
    Enemy *combatEnemy = nullptr;
    map<string, Audio> audios = {};
    int maxHP = 5, HP = maxHP;
    bool inCombat = false, dead = false, immune = false, mouseClicked = false;
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Object> &objects) override;
    void render(Vector2D Camera) override;
    void damage(int byPoints = 1);
    void attack();
    void resetPos(bool previous = true);
private:
    Cooldown throwCooldown = {1.0};
    map<string, Animation> anims = {};
    float speed = 0.0f, jumpStrength = 0.0f;
    void handleMovement(double dt);
    void handleAttacking(double dt);
};