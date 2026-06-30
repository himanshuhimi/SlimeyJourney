#include "../config.h"

const string TITLE = "Slimey Journey";
const float SPRITE_SIZE = 32.0f;
int WIDTH{640}, HEIGHT{360};
int CHANGED_WIDTH{WIDTH * 3}, CHANGED_HEIGHT{HEIGHT * 3};
int scaleX{CHANGED_WIDTH / WIDTH}, scaleY{CHANGED_HEIGHT / HEIGHT};
int CAMERA_X{WIDTH / 2}, CAMERA_Y{HEIGHT / 2};
bool scaled = true;
const vector<string> fruitTypes = {
    "apple", "blueberry",
    "mango", "melon",
    "orange", "strawberry"};
SDL_RendererLogicalPresentation logicalPresentation = SDL_LOGICAL_PRESENTATION_INTEGER_SCALE;

bool checkCollision(SDL_FRect A, SDL_FRect B) { return SDL_HasRectIntersectionFloat(&A, &B); }

SDL_FRect getMousePosition()
{
    SDL_FRect result = {0, 0, 0, 0};
    SDL_GetMouseState(&result.x, &result.y);
    if (scaled)
        result.x /= scaleX;
    result.y /= scaleY;
    return result;
}