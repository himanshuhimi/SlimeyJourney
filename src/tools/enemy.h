#pragma once

#include "sprite.h"
#include "LOS.h"
#include "ui/progress.h"
#include "sprites/ball.h"
#include "sprites/player.h"

class Enemy : public Sprite
{
public:
    Progress healthBar;
    Cooldown throwCooldown = {2.0};
    bool dead = false, hasAwarded = false, immune = false;
    map<string, Animation> anims;
    int speed = 0, maxHP = 0, HP = 0;
    string folderPath = "", type = "";
    vector<Ball> balls = {};
    Enemy(SDL_Renderer *renderer, string type,
        float x, float y, int speed = 40, int HP = 5, float sightRange = 40);
    void handle(double dt, const vector<Object> &grasses);
    void render(Vector2D Camera);
    void handleMovement(double dt);
    void damage(int byPoints = 1);
    template <typename T>
    void drop(vector<T> &vec)
    {
        if (!hasAwarded && dead)
        {
            vec.emplace_back(renderer, rect.x, rect.y);
            hasAwarded = true;
        }
    }
};