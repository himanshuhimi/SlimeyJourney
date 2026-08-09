#include "../group.h"
#include "../../entities/player.h"

template <class T>
Group<T>::Group() { sprites.clear(); }

template <class T>
void Group<T>::render(Player player, Vector2D Camera)
{
    for (auto &sprite : sprites)
        if (player.Position.distanceFromVec(sprite->Position) < WIDTH)
            sprite->render(Camera);
}

template <class T>
void Group<T>::handle(Player player, double dt, vector<Object> &grasses)
{
    for (auto &sprite : sprites)
        if (player.Position.distanceFromVec(sprite->Position) > WIDTH)
            sprite->handle(dt, grasses);
}

template <class T>
void Group<T>::insert(unique_ptr<T> sprite)
{
    sprites.emplace_back(std::move(sprite));
}

template <class T>
void Group<T>::kick(unique_ptr<T> sprite)
{
    auto it = std::find_if(
        sprites.begin(),
        sprites.end(),
        [&sprite](const std::unique_ptr<T> &ptr)
        {
            return ptr == sprite;
        }
    );
    if (it != sprites.end())
        sprites.erase(it);
}

template <class T>
int Group<T>::size() { return sprites.size(); }

template <class T>
typename Sprites<T>::iterator Group<T>::kick(unique_ptr<T> &&sprite)
{
    auto it = std::find_if(
        sprites.begin(),
        sprites.end(),
        [&sprite](const std::unique_ptr<Sprite> &ptr)
        {
            return ptr == sprite;
        }
    );
    if (it != sprites.end())
        return sprites.erase(it);
    else
        return sprites.end();
}

template <class T>
typename Sprites<T>::iterator Group<T>::begin() { return sprites.begin(); }

template <class T>
typename Sprites<T>::iterator Group<T>::end() { return sprites.end(); }