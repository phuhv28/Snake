#include "PlayGround.h"
#include <cassert>

PlayGround::PlayGround(int width, int height)
    : squares(height, std::vector<CellType>(width, CELL_EMPTY)),
      snake(this),
      status(GAME_RUNNING),
      score(0)
{
    addCherry();
}

void PlayGround::changeCellState(Position pos, CellType type)
{
    assert(pos.isInsideBox(0,0,getWidth(),getHeight()));

    squares[pos.y][pos.x] = type;
}

void PlayGround::nextStep()
{
    snake.nextStep();
}

void PlayGround::processUserInput(UserInput input)
{
    snake.processUserInput(input);
}