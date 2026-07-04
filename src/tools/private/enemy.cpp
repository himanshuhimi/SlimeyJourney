#include "../enemy.h"

EnemyData::EnemyData(
    float speed, float atkDmg, float atkRange, int maxHP,
    std::function<void(Vector2D)> atkMethod)
    : speed(speed), atkDmg(atkDmg), atkRange(atkRange), maxHP(maxHP), onAtk(atkMethod) {};

Enemy::Enemy(SDL_Renderer *renderer, float x, float y, string type, EnemyData data)
    : Sprite(renderer, "object.png", x, y), type(type), data(data)
{
    image = Image{renderer, "images/enemies/" + type + "/idle.png"};
    range = SDL_FRect{rect.x, rect.y, data.atkRange, data.atkRange};
    lineOfSight.rect.w = data.atkRange;
    HP = data.maxHP;
    anims = {
        {"walking", Animation(renderer, type + "/walking.png", 0.1)},
        {"damage", Animation(renderer, type + "/damage.png", 0.1)}};
    audios = {{"hurt", Audio(type + "/hurt.wav")}};
};

void Enemy::handle(double dt, const vector<Object> &objects)
{
    dead = HP <= 0;
    if (dead)
        return;
    Sprite::handle(dt, objects);
    range.x = rect.x - (range.w / 2) + (rect.w / 2);
    range.y = rect.y - (range.h / 2) + (rect.h / 2);
    if (anims.at("damage").active)
        anims.at("damage").handle(dt);
    if (Velocity.x && !anims.at("walking").active)
        anims.at("walking").restart();
    if (anims.at("walking").active)
        anims.at("walking").handle(dt);
}

void Enemy::render(Vector2D Camera)
{
    if (dead)
        return;
    if (anims.at("walking").active)
        anims.at("walking").render(Camera, rect);
    else
        Sprite::render(Camera);
    if (anims.at("damage").active)
        anims.at("damage").render(Camera, rect);
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

void Enemy::damage(int byPoints)
{
    if (dead)
        return;
    HP -= byPoints;
    anims.at("damage").restart();
    audios.at("hurt").play(Random.randint(50, 100));
}