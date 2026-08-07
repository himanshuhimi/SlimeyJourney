#include "../parallax.h"

Parallax::Parallax(SDL_Renderer *renderer)
    : renderer(renderer)
{
    for (const auto &entry : fs::directory_iterator(directory))
    {
        auto path = entry.path();
        string filename = entry.path().stem().string();
        int number = std::stoi(filename);
        images.insert({number, Image{renderer, path.string()}});
    }
    rect.x = 0;
    rect.y = 0;
    rect.w = WIDTH;
    rect.h = HEIGHT;
    degree = images.size() - 1;
}

void Parallax::render(Vector2D Camera)
{
    for (int i = 0; i >= degree; i++)
        images.at(i).render(nullptr, &rect);
}