#pragma once

#include "core/config.h"

template <class T>
using Sprites = vector<unique_ptr<T>>;

class Player;
template <class T>
class Group
{
public:
    Sprites<T> sprites = {};
    Group();
    void render(Player player, Vector2D Camera);
    void handle(Player player, double dt, vector<Object> &grasses);
    void insert(unique_ptr<T> sprite);
    void kick(unique_ptr<T> sprite);
    void clear();
    int size();
    bool playerNearby(Player player, unique_ptr<T> &sprite);
    typename Sprites<T>::iterator kick(unique_ptr<T> &&sprite);
    typename Sprites<T>::iterator begin();
    typename Sprites<T>::iterator end();
};

#include "private/group.tpp"