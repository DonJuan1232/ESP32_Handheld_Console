#ifndef SNAKE_H
#define SNAKE_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <vector>

#include "vector2.h"

class snake {
   public:
    snake();
    void moveSnake();
    void changeDirection(int newDir);
    void drawSnake(Adafruit_ILI9341 &display);
    void makeLonger();
    Vector2 getLocation();
    bool checkSelfCollision();

   private:
    std::vector<Vector2> body;  // snake body segments
    int direction;              // 0=Up, 1=Down, 2=Left, 3=Right
};

#endif
