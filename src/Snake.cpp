#include "PlayGround.h"

Snake::Snake(PlayGround *playGround)
    : position(playGround->getWidth() / 2, playGround->getHeight() / 2),
      playGround(playGround),
      direction(Direction::RIGHT)
{
    playGround->changeCellState(position, CELL_SNAKE);
}

void Snake::processUserInput(UserInput input)
{
    direction = changeDirection(input);
}

Direction Snake::changeDirection(UserInput input)
{
    switch (input)
    {
    case KEY_UP:
        return direction != DOWN ? UP : direction;
    case KEY_DOWN:
        return direction != UP ? DOWN : direction;
    case KEY_LEFT:
        return direction != RIGHT ? LEFT : direction;
    case KEY_RIGHT:
        return direction != LEFT ? RIGHT : direction;
    default:
        return direction;
    }
}

void Snake::nextStep()
{
    Position newPosition = position.move(direction);
    playGround->changeCellState(position, CELL_EMPTY);
    position = newPosition;
    playGround->changeCellState(position, CELL_SNAKE);
}
