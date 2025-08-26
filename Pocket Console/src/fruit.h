#ifndef FRUIT_H
#define FRUIT_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <vector>

#include "vector2.h"

class fruit {
   public:
    // fruit();
    void spawnFruit(Adafruit_ILI9341 &display);
    void drawFruit(Adafruit_ILI9341 &display);
    void deleteFruit(Adafruit_ILI9341 &display);
    Vector2 location;
};

#endif