#include <SDL.h>
#include <chrono>
#include <string>
#include <iostream>

#define CLOCK_NOW std::chrono::system_clock::now

const SDL_Color CYAN_COLOR = {0, 255, 255};
const SDL_Color BLUE_COLOR = {0, 0, 255};
const SDL_Color ORANGE_COLOR = {255, 165, 0};
const SDL_Color YELLOW_COLOR = {255, 255, 0};
const SDL_Color LIME_COLOR = {0, 255, 0};
const SDL_Color PURPLE_COLOR = {128, 0, 128};
const SDL_Color RED_COLOR = {255, 0, 0};
const SDL_Color WHITE_COLOR = {255, 255, 255};
const SDL_Color BLACK_COLOR = {0, 0, 0};
const SDL_Color GREEN_COLOR = {0, 128, 0};

const SDL_Color DEFAULT_COLOR = BLACK_COLOR;

const double STEP_DELAY = 0.1;

typedef std::chrono::duration<double> ElapsedTime;

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;

const int BOARD_WIDTH = 30;
const int BOARD_HEIGHT = 20;

const int CELL_SIZE = 30;

const std::string WINDOW_TITLE = "Snake Game";