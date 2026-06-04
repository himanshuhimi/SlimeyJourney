#pragma once

#include "sprite.h"
#include "LOS.h"
#include "../ui/progress.h"

class Enemy : public Sprite
{
public:
    Progress healthBar;
    bool dead = false, hasAwarded = false, immune = false;
    unordered_map<string, Animation *> anims;
    int speed = 0, maxHP = 0, HP = 0;
    string folderPath = "", type = "";
    Enemy(SDL_Renderer *renderer, string type,
        float x, float y, int speed = 40, int HP = 5);
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
    void damage(int byPoints = 1);
    void handleShooting(double dt);
    template <typename T>
    void drop(vector<T> &vec)
    {
        if (!hasAwarded && dead)
        {
            vec.emplace_back(T(renderer, rect.x, rect.y));
            hasAwarded = true;
        }
    }
};