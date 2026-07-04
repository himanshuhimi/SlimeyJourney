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
    virtual void handle(double dt, const vector<Object> &objects);
    virtual void render(Vector2D Camera);
    virtual void damage(int byPoints = 1);
    template <typename T>
    void drop(vector<unique_ptr<T>> &droppingList)
    {
        droppingList.emplace_back(make_unique<T>(renderer, Center.x, Center.y));
    }
};