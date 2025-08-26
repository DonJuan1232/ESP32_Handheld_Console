#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdlib.h>

#include "driver/gpio.h"
#include "esp_system.h"
#include "fruit.h"
#include "snake.h"
// Pin definitions
#define TFT_CS 2
#define TFT_DC 17
#define TFT_RST 16
#define LED_PIN 2
#define BUTTON_PIN_RIGHT 13
#define BUTTON_PIN_LEFT 12
#define BUTTON_PIN_DOWN 27
#define BUTTON_PIN_UP 14
#define BUTTON_PIN_A 25
#define BUTTON_PIN_B 26

Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

const gpio_num_t buttonPinA = (gpio_num_t)BUTTON_PIN_A;
const gpio_num_t buttonPinB = (gpio_num_t)BUTTON_PIN_B;
const gpio_num_t buttonPinUp = (gpio_num_t)BUTTON_PIN_UP;
const gpio_num_t buttonPinDown = (gpio_num_t)BUTTON_PIN_DOWN;
const gpio_num_t buttonPinLeft = (gpio_num_t)BUTTON_PIN_LEFT;
const gpio_num_t buttonPinRight = (gpio_num_t)BUTTON_PIN_RIGHT;

uint8_t upStateA = 0;
uint8_t upStateB = 0;
uint8_t upStateUp = 0;
uint8_t upStateDown = 0;
uint8_t upStateLeft = 0;
uint8_t upStateRight = 0;

snake gameSnake;
fruit gameFruit;

void resetGame() {
    display.fillScreen(ILI9341_BLACK);
    gameSnake = snake();  // re-initialize
    gameFruit.spawnFruit(display);
}

void GameTask(void *pvParameters) {
    while (true) {
        Vector2 head = gameSnake.getLocation();

        // check boundaries
        if (head.x * 10 <= 0 || head.x * 10 >= 250 || head.y * 10 <= 0 || head.y * 10 >= 240) {
            resetGame();
        }

        // check self collision
        if (gameSnake.checkSelfCollision()) {
            resetGame();
        }

        // check fruit collision
        if ((head.x * 10) >= gameFruit.location.x - 10 && (head.x * 10) <= gameFruit.location.x + 10 &&
            (head.y * 10) >= gameFruit.location.y - 10 && (head.y * 10) <= gameFruit.location.y + 10) {
            gameFruit.deleteFruit(display);
            gameFruit.spawnFruit(display);
            gameSnake.makeLonger();
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void DisplayTask(void *pvParameters) {
    while (true) {
        display.fillScreen(ILI9341_BLACK);  // clear screen

        // Move snake every frame
        gameSnake.moveSnake();
        gameFruit.drawFruit(display);

        // Draw snake
        gameSnake.drawSnake(display);

        vTaskDelay(pdMS_TO_TICKS(100));  // snake speed (200ms per move)
    }
}
void ButtonTask(void *pvParameters) {
    const gpio_num_t ledPin = (gpio_num_t)LED_PIN;

    while (true) {
        // Read button
        upStateA = gpio_get_level(buttonPinA);
        upStateB = gpio_get_level(buttonPinB);
        upStateUp = gpio_get_level(buttonPinUp);
        upStateDown = gpio_get_level(buttonPinDown);
        upStateLeft = gpio_get_level(buttonPinLeft);
        upStateRight = gpio_get_level(buttonPinRight);

        if (upStateUp) gameSnake.changeDirection(0);
        if (upStateDown) gameSnake.changeDirection(1);
        if (upStateLeft) gameSnake.changeDirection(2);
        if (upStateRight) gameSnake.changeDirection(3);

        // Optional: delay to prevent CPU hogging
        vTaskDelay(pdMS_TO_TICKS(10));  // 10 ms
    }
}

void setup() {
    // Initialize display
    display.begin();

    uint32_t seed = esp_random();
    randomSeed(seed);
    Serial.begin(9600);

    // Configure GPIO13 as input with pull-down
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << BUTTON_PIN_A) | (1ULL << BUTTON_PIN_B) | (1ULL << BUTTON_PIN_UP) | (1ULL << BUTTON_PIN_DOWN) | (1ULL << BUTTON_PIN_LEFT) | (1ULL << BUTTON_PIN_RIGHT);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);

    // Configure GPIO2 as output
    gpio_set_direction((gpio_num_t)LED_PIN, GPIO_MODE_OUTPUT);

    // Draw initial graphics
    display.fillScreen(ILI9341_BLACK);

    gameFruit.spawnFruit(display);

    // Create FreeRTOS tasks
    xTaskCreate(ButtonTask, "ButtonTask", 2048, NULL, 1, NULL);
    xTaskCreate(DisplayTask, "DisplayTask", 4096, NULL, 1, NULL);
    xTaskCreate(GameTask, "GameTask", 4096, NULL, 1, NULL);
}

void loop() {
    // No need to do anything here with FreeRTOS
}
