#pragma once

#include "../tools/sprite.h"
#include "grass.h"

class Player : public Sprite
{
public:
    int speed;
    float jumpStrength;
    bool spacePressed = false;
    std::unordered_map<string, Animation *> anims;
    std::unordered_map<string, Audio *> audios;
    enum Direction {Left, Right} lastDirection;
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Grass> &grasses);
    void render(Vector2D Camera);
};