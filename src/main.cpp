#include <iostream>
#include <ctime>
#include <SDL.h>
#include <chrono>
#include <vector>

#include <PlayGround.h>

// tên ngắn của hàm lấy thời gian
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

void initSDL(SDL_Window *&window, SDL_Renderer *&renderer);
void renderSplashScreen();
void waitUntilKeyPressed();
void renderplaygroundPlay(SDL_Renderer *renderer, const PlayGround &playGround);
void renderplaygroundOver(SDL_Renderer *renderer, const PlayGround &playGround);
UserInput interpretEvent(SDL_Event e);
void updateRankingTable(const PlayGround &playGround);
void quitSDL(SDL_Window *window, SDL_Renderer *renderer);

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    initSDL(window, renderer);

    renderSplashScreen();
    PlayGround playGround(BOARD_WIDTH, BOARD_HEIGHT);
    SDL_Event e;
    renderplaygroundPlay(renderer, playGround);
    auto start = CLOCK_NOW();
    while (playGround.isGameRunning())
    {
        while (SDL_PollEvent(&e) != 0)
        {
            UserInput input = interpretEvent(e);
            playGround.processUserInput(input);
        } // non-blocking event detection
        auto end = CLOCK_NOW();
        ElapsedTime elapsed = end - start;
        if (elapsed.count() > STEP_DELAY)
        {
            playGround.nextStep();
            renderplaygroundPlay(renderer, playGround);
            start = end;
        }
        SDL_Delay(1); // to prevent high CPU usage because of SDL_PollEvent()
    }
    renderplaygroundOver(renderer, playGround);
    updateRankingTable(playGround);
    quitSDL(window, renderer);

    return 0;
}

void logSDLError(std::ostream &os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal)
    {
        SDL_Quit();
        exit(1);
    }
}
void initSDL(SDL_Window *&window, SDL_Renderer *&renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    // window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (window == nullptr)
        logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr)
        logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}
void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true)
    {
        if (SDL_WaitEvent(&e) != 0 &&
            (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(100);
    }
}
void renderSplashScreen()
{
    waitUntilKeyPressed();
}
void renderplaygroundPlay(SDL_Renderer *renderer, const PlayGround &playGround)
{
    int top = 0, left = 0;
    SDL_SetRenderDrawColor(renderer, PURPLE_COLOR.r, PURPLE_COLOR.g, PURPLE_COLOR.b, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, WHITE_COLOR.r, WHITE_COLOR.g, WHITE_COLOR.b, 0);
    for (int i = 0; i <= BOARD_WIDTH; i++)
        SDL_RenderDrawLine(renderer, left + i * CELL_SIZE, 0, left + i * CELL_SIZE, SCREEN_HEIGHT);

    for (int i = 0; i <= BOARD_HEIGHT; i++)
        SDL_RenderDrawLine(renderer, 0, top + i * CELL_SIZE, SCREEN_WIDTH, top + i * CELL_SIZE);

    const std::vector<std::vector<CellType>> &squares = playGround.getSquares();

    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            if (squares[i][j] == CELL_CHERRY)
            {
                SDL_SetRenderDrawColor(renderer, RED_COLOR.r, RED_COLOR.g, RED_COLOR.b, 0);
                SDL_RenderDrawLine(renderer, left + j * CELL_SIZE + 5, top + i * CELL_SIZE + 5, left + j * CELL_SIZE + 5 + CELL_SIZE - 10, top + i * CELL_SIZE + 5);
                SDL_RenderDrawLine(renderer, left + j * CELL_SIZE + 5 + CELL_SIZE - 10, top + i * CELL_SIZE + 5, left + j * CELL_SIZE + 5 + CELL_SIZE - 10, top + i * CELL_SIZE + 5 + CELL_SIZE - 10);
                SDL_RenderDrawLine(renderer, left + j * CELL_SIZE + 5 + CELL_SIZE - 10, top + i * CELL_SIZE + 5 + CELL_SIZE - 10, left + j * CELL_SIZE + 5, top + i * CELL_SIZE + 5 + CELL_SIZE - 10);
                SDL_RenderDrawLine(renderer, left + j * CELL_SIZE + 5, top + i * CELL_SIZE + 5 + CELL_SIZE - 10, left + j * CELL_SIZE + 5, top + i * CELL_SIZE + 5);
                // std::cout << j << ' ' << i << std::endl;
            }
            else if (squares[i][j] == CELL_SNAKE)
            {
                SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, 0);
                SDL_RenderDrawLine(renderer, left + j * CELL_SIZE + 5, top + i * CELL_SIZE + 5, left + j * CELL_SIZE + 5 + CELL_SIZE - 10, top + i * CELL_SIZE + 5);
                SDL_RenderDrawLine(renderer, left + j * CELL_SIZE + 5 + CELL_SIZE - 10, top + i * CELL_SIZE + 5, left + j * CELL_SIZE + 5 + CELL_SIZE - 10, top + i * CELL_SIZE + 5 + CELL_SIZE - 10);
                SDL_RenderDrawLine(renderer, left + j * CELL_SIZE + 5 + CELL_SIZE - 10, top + i * CELL_SIZE + 5 + CELL_SIZE - 10, left + j * CELL_SIZE + 5, top + i * CELL_SIZE + 5 + CELL_SIZE - 10);
                SDL_RenderDrawLine(renderer, left + j * CELL_SIZE + 5, top + i * CELL_SIZE + 5 + CELL_SIZE - 10, left + j * CELL_SIZE + 5, top + i * CELL_SIZE + 5);
                
                // std::cout << j << ' ' << i << std::endl;
            }
        }
    }

    SDL_RenderPresent(renderer);
}
void renderplaygroundOver(SDL_Renderer *renderer, const PlayGround &playGround) {}
UserInput interpretEvent(SDL_Event e)
{
    if (e.type == SDL_KEYUP)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            return KEY_UP;
        case SDLK_w:
            return KEY_UP;
        case SDLK_DOWN:
            return KEY_DOWN;
        case SDLK_s:
            return KEY_DOWN;
        case SDLK_LEFT:
            return KEY_LEFT;
        case SDLK_a:
            return KEY_LEFT;
        case SDLK_RIGHT:
            return KEY_RIGHT;
        case SDLK_d:
            return KEY_RIGHT;
        }
    }
    return NO_INPUT;
}
void updateRankingTable(const PlayGround &playGround) {}
void quitSDL(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}