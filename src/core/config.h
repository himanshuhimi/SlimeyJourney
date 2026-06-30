#pragma once

#define SDL_MAIN_HANDLED

#include <iostream>
#include <algorithm>
#include <filesystem>
#include <functional>
#include <random>
#include <vector>
#include <cmath>
#include <map>
#include <variant>
#include <sqlite3.h>
#include <tinyxml2.h>
#include "engine/defines.h"
#include "engine/graphics.h"
#include "engine/util.h"
#include "engine/vector.h"

using std::string, std::vector, std::map;
namespace fs = std::filesystem;

extern const string TITLE;
extern const float SPRITE_SIZE;
extern int WIDTH, HEIGHT;
extern int CHANGED_WIDTH, CHANGED_HEIGHT;
extern int scaleX, scaleY;
extern int CAMERA_X, CAMERA_Y;
extern bool scaled;
extern const vector<string> fruitTypes;
extern SDL_RendererLogicalPresentation logicalPresentation;

template <typename T>
void print(const T &message) { std::cout << "[LOG] " << message << std::endl; };
bool checkCollision(SDL_FRect A, SDL_FRect B);
SDL_FRect getMousePosition();