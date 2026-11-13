#include "../group.h"
#include "../../entities/player.h"

template <typename T>
Group<T>::Group() { sprites.clear(); }

template <typename T>
void Group<T>::render(Player player, Vector2D Camera)
{
    for (auto &sprite : sprites)
        if (playerNearby(player, sprite))
            sprite->render(Camera);
}

template <typename T>
void Group<T>::handle(Player player, double dt, vector<Object> &grasses)
{
    for (auto &sprite : sprites)
        if (playerNearby(player, sprite))
            sprite->handle(dt, grasses);
}

template <typename T>
void Group<T>::insert(unique_ptr<T> sprite)
{
    sprites.emplace_back(std::move(sprite));
}

template <typename T>
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

template <typename T>
void Group<T>::clear() { sprites.clear(); }

template <typename T>
int Group<T>::size() { return sprites.size(); }

template <typename T>
bool Group<T>::playerNearby(Player player, unique_ptr<T> &sprite)
{
    int renderDistance = WIDTH / 2;
    return player.Position.distanceFromVec(sprite->Position, true) < pow(renderDistance, 2);
}

template <typename T>
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

template <typename T>
typename Sprites<T>::iterator Group<T>::begin() { return sprites.begin(); }

template <typename T>
typename Sprites<T>::iterator Group<T>::end() { return sprites.end(); }