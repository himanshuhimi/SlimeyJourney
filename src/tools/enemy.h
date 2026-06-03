#pragma once

#include "sprite.h"
#include "LOS.h"
#include "../ui/progress.h"

class Enemy : public Sprite
{
public:
    bool hasDied = false, hasAwarded = false;
    int speed = 0, maxHealthPoints = 0, healthPoints = 0;
    string foldertype = "";
    Progress healthBar;
    Enemy(
        SDL_Renderer *renderer, 
        string foldertype, 
        float x, 
        float y, 
        int speed,
        int healthPoints = 5
    );
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
    void damage(int byPoints = 1);
    template <typename T>
    void drop(vector<T> &vec)
    {
        if (!hasAwarded && healthPoints <= 0)
        {
            vec.emplace_back(T(renderer, Position.x, Position.y));
            hasAwarded = true;
        }
    }
};