#pragma once

#include "sprite.h"
#include "LOS.h"

class Enemy : public Sprite
{
public:
    int speed = 0;
    string foldertype = "";
    Enemy(SDL_Renderer *renderer, string foldertype, float x, float y, int speed);
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
};