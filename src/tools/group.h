#pragma once

#include "core/config.h"

template <typename T>
using Sprites = vector<unique_ptr<T>>;

struct Player;
template <typename T>
struct Group
{

    Sprites<T> sprites = {};
    Player *player = nullptr;
    Group(Player *player);
    void render(Vector2D Camera);
    void handle(double dt, vector<Object> &grasses);
    void insert(unique_ptr<T> sprite);
    void kick(unique_ptr<T> sprite);
    void clear();
    int size();
    bool playerNearby(unique_ptr<T> &sprite);
    typename Sprites<T>::iterator kick(unique_ptr<T> &&sprite);
    typename Sprites<T>::iterator begin();
    typename Sprites<T>::iterator end();
};

#include "private/group.tpp"