#pragma once

#include "../sprite.h"
#include "../interval.h"
#include "weapon.h"

struct EnemyData
{
    float speed = 0;
    float atkRange = 0;
    float alertRange = 0;
    int maxHP = 0;
    bool rangeVisible = false;
    std::function<void(Vector2D)> onAtk;
    EnemyData(
        float speed = 0.0f,
        float atkRange = 0.0f,
        int maxHP = 0,
        std::function<void(Vector2D)> atkMethod = [](Vector2D Direction) {});
};

struct Enemy : public Sprite
{

    SDL_FRect range;
    EnemyData data;
    struct EnemyActions
    {
        bool alert = false, idling = false, attacking = false;
    } actions;
    Cooldown atkCooldown = {3.0f};
    Interval *moveTimer;
    map<string, Anim> anims = {};
    map<string, Audio> audios = {};
    vector<unique_ptr<Weapon>> throws = {};
    float travelDistance = 0.0;
    Vector2D travelVector;
    string type = "";
    bool dead = false;
    int HP = 0;
    Enemy(SDL_Renderer *renderer, float x, float y, string type, EnemyData data);
    virtual void handle(double dt, const vector<Object> &objects);
    virtual void render(Vector2D Camera);
    virtual void damage(int byPoints = 1);
    virtual void attack(Vector2D Direction);
};