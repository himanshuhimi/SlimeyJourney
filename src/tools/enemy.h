#pragma once

#include "sprite.h"
#include "LOS.h"
#include "../ui/progress.h"

class Enemy : public Sprite
{
public:
    Progress healthBar;
    bool hasDied = false, hasAwarded = false;
    int speed = 0, maxHP = 0, HP = 0;
    string type = "";
    Enemy(
        SDL_Renderer *renderer, string type,
        float x, float y, int speed = 40, int HP = 5);
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
    void damage(int byPoints = 1);
    template <typename T>
    void drop(vector<T> &vec)
    {
        if (!hasAwarded && hasDied)
        {
            vec.emplace_back(T(renderer, Position.x, Position.y));
            hasAwarded = true;
        }
    }
};