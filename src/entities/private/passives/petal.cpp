#include "../../passives/petal.h"

Petal::Petal(SDL_Renderer *renderer)
    : Sprite(renderer, "empty.png", Random.randint(0, WIDTH), Random.randint(0, HEIGHT))
{
    Velocity.x = Random.randint(1, 10);
    Velocity.x *= Random.randint(0, 1) ? -1 : 1;
    Velocity.y = Random.randint(1, 10);
    vector<string> petals = {};
    for (auto &entry : fs::directory_iterator("data/assets/images/passives/petals/")) 
        petals.emplace_back(entry.path().stem().string());
    choice = Random.choice<string>(petals);
    image = Image(
        renderer, 
        "images/passives/petals/" + choice + ".png",
        Random.randint(0, 180)
    );
    rect.w /= 2;
    rect.h /= 2;
}

void Petal::handle(double dt)
{
    Position.x += Velocity.x * dt;
    rect.x = Position.x;
    Position.y += Velocity.y * dt;
    rect.y = Position.y;
}