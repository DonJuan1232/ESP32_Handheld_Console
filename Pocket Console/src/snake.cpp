#include "snake.h"

snake::snake() {
    direction = 3;             // start moving right
    body.push_back({10, 10});  // head
    body.push_back({9, 10});   // tail segment
}

void snake::changeDirection(int newDir) {
    // Prevent reversing direction directly
    if ((direction == 0 && newDir == 1) ||
        (direction == 1 && newDir == 0) ||
        (direction == 2 && newDir == 3) ||
        (direction == 3 && newDir == 2)) {
        return;
    }
    direction = newDir;
}

void snake::moveSnake() {
    // Copy head
    Vector2 newHead = body[0];

    // Move head depending on direction
    if (direction == 0) newHead.y -= 1;  // Up
    if (direction == 1) newHead.y += 1;  // Down
    if (direction == 2) newHead.x -= 1;  // Left
    if (direction == 3) newHead.x += 1;  // Right

    // Insert new head
    body.insert(body.begin(), newHead);

    // Remove last segment (unless eating food)
    body.pop_back();
}

void snake::drawSnake(Adafruit_ILI9341 &display) {
    for (auto &segment : body) {
        display.fillRect(segment.x * 10, segment.y * 10, 10, 10, ILI9341_GREEN);
    }
}

void snake::makeLonger() {
    if (!body.empty()) {
        // Duplicate the last segment
        Vector2 tail = body.back();
        body.push_back(tail);
    }
}

bool snake::checkSelfCollision() {
    if (body.empty()) return false;

    Vector2 head = body[0];
    for (size_t i = 1; i < body.size(); i++) {
        if (body[i].x == head.x && body[i].y == head.y) {
            return true;
        }
    }
    return false;
}

Vector2 snake::getLocation() {
    return body[0];
}
