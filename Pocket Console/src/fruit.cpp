#include "fruit.h"

uint16_t xcord;
uint16_t ycord;
void fruit::spawnFruit(Adafruit_ILI9341 &display) {
    xcord = rand() % 250;  // TODO: change to 320 for full screen
    ycord = rand() % 240;

    location.x = xcord;
    location.y = ycord;

    display.fillRect(xcord, ycord, 10, 10, ILI9341_BLUE);
}

void fruit::drawFruit(Adafruit_ILI9341 &display) {
    display.fillRect(xcord, ycord, 10, 10, ILI9341_BLUE);
}

void fruit::deleteFruit(Adafruit_ILI9341 &display) {
    display.fillRect(xcord, ycord, 10, 10, ILI9341_BLACK);
}