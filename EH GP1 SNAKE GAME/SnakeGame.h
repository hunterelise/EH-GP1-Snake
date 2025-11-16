#pragma once
#include <vector>
#include <memory>
#include "Point.h"
#include "Food.h"

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Segment {
    Point p;
    Segment(int x, int y) : p(x, y) {}
};

// Snake game class
class SnakeGame {
private:
    int width = 40;
    int height = 20;
    int targetFps = 10;
    int score = 0;
    int lives = 3;
    Direction direction = RIGHT;
    bool running = false;
    std::vector<Segment*> segments;
    std::unique_ptr<Food> food = nullptr;
    std::unique_ptr<Point> extraLife = nullptr;

    // Snake manipulation
    void addHead(const Point& p);
    void popTail();
    void clearSegments();
    bool isOccupied(const Point& p) const;

    // Game objects
    void spawnFood();
    void spawnExtraLife();

    // Game logic
    void handleInput();
    void update();
    void renderFrame();

public:
    SnakeGame(int w = 40, int h = 20, int fps = 10);
    ~SnakeGame();

    void init(); // Initialize game
    void run();  // Main game loop
};
