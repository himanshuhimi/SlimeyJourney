#include "../parallax.h"

Parallax::Parallax(SDL_Renderer *renderer)
    : renderer(renderer)
{
    for (const auto &entry : fs::directory_iterator(directory))
    {
        string filename = entry.path().stem().string();
        int number = std::stoi(filename);
        images.insert({number, Image{renderer, "ui/parallax/" + filename + ".png"}});
    }
    rect.x = 0;
    rect.y = 0;
    rect.w = WIDTH;
    rect.h = HEIGHT;
    degree = images.size();
}

void Parallax::render(Vector2D Camera)
{
    SDL_FRect dst = rect;
    dst.x -= Camera.x;
    dst.y -= Camera.y;
    for (int i = 0; i < degree; i++)
        images.at(i).render(nullptr, &rect);
}