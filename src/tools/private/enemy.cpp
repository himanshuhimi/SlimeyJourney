#include "../enemy.h"

EnemyData::EnemyData(
    float speed, float atkDmg, float atkRange, int maxHP,
    std::function<void(Vector2D)> atkMethod)
    : speed(speed), atkDmg(atkDmg), atkRange(atkRange), maxHP(maxHP), onAtk(atkMethod) {};

Enemy::Enemy(SDL_Renderer *renderer, float x, float y, string type, EnemyData data)
    : Sprite(renderer, "object.png", x, y), type(type), data(data)
{
    image = Image{renderer, "images/enemies/" + type + "/idle.png"};
    range = SDL_FRect{
        x - image.width / 2, y - image.height / 2,
        data.atkRange, data.atkRange
    };
    lineOfSight.rect.w = data.atkRange;
    HP = data.maxHP;
};

void Enemy::handle(double dt, const vector<Object> objects)
{
    dead = HP <= 0;
    if (dead)
        return;
    Sprite::handle(dt, objects);
    range.x = rect.x - (rect.w / 2);
    range.y = rect.y - (rect.h / 2);
}

void Enemy::render(Vector2D Camera)
{
    if (dead)
        return;
    Sprite::render(Camera);
    if (data.rangeVisible)
    {
        SDL_SetRenderDrawColor(renderer, 
            colors.red.r, 
            colors.red.g, 
            colors.red.b, 
            colors.red.a);
        SDL_FRect rangeDst = range;
        rangeDst.x -= Camera.x;
        rangeDst.y -= Camera.y;
        SDL_RenderRect(renderer, &rangeDst);
    }
}