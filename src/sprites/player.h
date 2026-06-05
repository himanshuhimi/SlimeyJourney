#pragma once

#include "../tools/sprite.h"
#include "grass.h"
#include "ball.h"
#include "../ui/progress.h"

class Enemy;
class Player : public Sprite
{
public:
    Enemy *combatEnemy = nullptr;
    Progress healthBar;
    Vector2D prevPos;
    Cooldown throwCooldown = {1.0};
    bool inCombat = false, dead = false, immune = false, mouseClicked = false;
    map<string, Animation> anims;
    map<string, Audio> audios;
    vector<Ball> balls = {};
    int maxHP = 5, HP = maxHP;
    float speed, jumpStrength;
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
    void damage(int byPoints = 1);
    void resetPos(bool previous = true);
    void handleMovement(double dt);
    void handleShooting(double dt);
};