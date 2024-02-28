#ifndef SNAKE_H
#define SNAKE_H

#include "Position.h"

class PlayGround;

enum UserInput
{
    NO_INPUT = 0,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT
};

class Snake
{
private:
    PlayGround *playGround;
    Position position;
    Direction direction;

public:
    Snake(PlayGround *playGround);
    void processUserInput(UserInput input);
    Direction changeDirection(UserInput input);
    void nextStep();
};
#endif