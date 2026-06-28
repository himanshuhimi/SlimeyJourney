#pragma once

#include "tools/sprite.h"

struct EnemyData
{
    float speed = 0;
    float atkDmg = 0;
    float atkRange = 0;
    float alertRange = 0;
    int maxHP = 0;
    bool rangeVisible = false;
    std::function<void(Vector2D)> onAtk;
    EnemyData(
        float speed = 0.0f,
        float atkDmg = 0.0f,
        float atkRange = 0.0f,
        int maxHP = 0,
        std::function<void(Vector2D)> atkMethod = [](Vector2D Direction) {});
};

class Enemy : public Sprite
{
public:
    SDL_FRect range;
    EnemyData data;
    struct EnemyActions
    {
        bool alert = false, attacking = false;
    } actions;
    Cooldown atkCooldown = {3.0f};
    map<string, Animation> anims = {};
    map<string, Audio> audios = {};
    bool dead = false;
    int HP = 0;
    string type = "";
    Enemy(SDL_Renderer *renderer, float x, float y, string type, EnemyData data);
    void handle(double dt, const vector<Object> objects);
    void render(Vector2D Camera);
    void damage(int byPoints = 1);
    template <typename T>
    void drop(vector<T> droppingList)
    {
        droppingList.emplace_back(renderer, Center.x, Center.y);
    }
};