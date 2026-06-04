#pragma once

#include "../tools/sprite.h"
#include "grass.h"
#include "ball.h"
#include "../ui/progress.h"

class Player : public Sprite
{
public:
    Progress healthBar;
    Vector2D prevPos, original;
    Cooldown throwCooldown = {1.0};
    bool dead, immune, mouseClicked;
    unordered_map<string, Animation *> anims;
    unordered_map<string, Audio *> audios;
    vector<Ball> balls = {};
    int HP = 5;
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
    void damage(int byPoints = 1);
    void resetPos(bool previous = true);
    void handleJump(double dt, const bool *keys);
    void handleShooting(double dt);
};