#include "SnakeGame.h"
#include "ConsoleUtils.h"
#include <conio.h>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <windows.h>

using namespace std;
using namespace std::chrono;

SnakeGame::SnakeGame(int w, int h, int fps)
    : width(w), height(h), targetFps(fps), running(false), score(0), direction(RIGHT)
{
    srand((unsigned)time(nullptr));
}

SnakeGame::~SnakeGame() {
    clearSegments();
}

void SnakeGame::init() {
    clearSegments();
    running = true;
    direction = RIGHT;
    score = 0;
    lives = 3; // 3 lives
    extraLife.reset(); // clear extra life

    int startX = width / 2;
    int startY = height / 2;
    addHead(Point(startX - 2, startY));
    addHead(Point(startX - 1, startY));
    addHead(Point(startX, startY));

    spawnFood();
    hideCursor();
    renderFrame();
}

void SnakeGame::run() {
    milliseconds frameTime((int)(1000.0 / targetFps));

    while (running) {
        auto start = high_resolution_clock::now();

        handleInput();
        update();
        renderFrame();

        auto elapsed = high_resolution_clock::now() - start;
        if (elapsed < frameTime)
            this_thread::sleep_for(frameTime - elapsed);
    }

    // Flash red and show GAME OVER below the grid
    for (int i = 0; i < 5; ++i) {
        setColor(4); // red
        setCursor(0, height + 3);
        cout << "====== GAME OVER ======\n";
        this_thread::sleep_for(milliseconds(200));

        setColor(7); // default
        setCursor(0, height + 3);
        cout << string(21, ' ') << "\r"; // clear line
        this_thread::sleep_for(milliseconds(200));
    }

    setColor(4);
    setCursor(0, height + 3);
    cout << "====== GAME OVER ======\n";
    setColor(7);
    cout << "Final Score: " << score << "\n";
    cout << "Press Q to return to menu...";

    while (true) {
        int ch = _getch();
        if (ch == 'q' || ch == 'Q') break;
    }
}

void SnakeGame::addHead(const Point& p) {
    segments.insert(segments.begin(), new Segment(p.x, p.y));
}

void SnakeGame::popTail() {
    if (!segments.empty()) {
        delete segments.back();
        segments.pop_back();
    }
}

void SnakeGame::clearSegments() {
    for (auto s : segments)
        delete s;
    segments.clear();
}

void SnakeGame::spawnFood() {
    while (true) {
        int fx = rand() % width;
        int fy = rand() % height;
        Point fp(fx, fy);

        // Skip positions on or adjacent to walls
        if (fx <= 1 || fx >= width - 2 || fy <= 1 || fy >= height - 2)
            continue;

        if (!isOccupied(fp) && (!extraLife || !(fp == *extraLife))) {
            food = std::make_unique<Food>(fx, fy);

            if (lives < 3 && (rand() % 5 == 0)) // 20% chance and only spawn if < 3 lives
                spawnExtraLife();

            break;
        }
    }
}


void SnakeGame::spawnExtraLife() {
    while (true) {
        int x = rand() % width;
        int y = rand() % height;
        Point p(x, y);
        if (!isOccupied(p) && (!food || !(food->pos == p))) {
            extraLife = make_unique<Point>(x, y);
            break;
        }
    }
}

bool SnakeGame::isOccupied(const Point& p) const {
    for (auto s : segments)
        if (s->p == p)
            return true;
    return false;
}

void SnakeGame::handleInput() {
    if (!_kbhit()) return;

    int ch = _getch();

    if (ch == 0 || ch == 224) {
        int key = _getch();
        switch (key) {
        case 72: if (direction != DOWN) direction = UP; break;
        case 80: if (direction != UP) direction = DOWN; break;
        case 75: if (direction != RIGHT) direction = LEFT; break;
        case 77: if (direction != LEFT) direction = RIGHT; break;
        }
    }
    else {
        if (ch == 'q' || ch == 'Q') running = false;
    }
}

void SnakeGame::update() {
    Point newHead = segments.front()->p;

    switch (direction) {
    case UP:    newHead.y -= 1; break;
    case DOWN:  newHead.y += 1; break;
    case LEFT:  newHead.x -= 1; break;
    case RIGHT: newHead.x += 1; break;
    }

    // Collision: walls or self
    if (newHead.x < 0 || newHead.x >= width || newHead.y < 0 || newHead.y >= height || isOccupied(newHead)) {
        lives--;
        if (lives >= 0) {
            // Flash red 3 times (even on last life)
            for (int i = 0; i < 3; ++i) {
                setColor(4); // red
                renderFrame();
                this_thread::sleep_for(milliseconds(150));

                setColor(7); // default
                renderFrame();
                this_thread::sleep_for(milliseconds(150));
            }

            if (lives > 0) {
                this_thread::sleep_for(milliseconds(500));

                // Reset snake to center
                clearSegments();
                direction = RIGHT;
                int startX = width / 2;
                int startY = height / 2;
                addHead(Point(startX - 2, startY));
                addHead(Point(startX - 1, startY));
                addHead(Point(startX, startY));
            }
            else {
                running = false;
            }
        }
        return;
    }

    addHead(newHead);

    // Check food
    if (food && newHead == food->pos) {
        score += 10;
        spawnFood();
    }
    else {
        popTail();
    }

    // Check extra life pickup
    if (extraLife && newHead == *extraLife) {
        lives++; // gain 1 life
        extraLife.reset();

        // Flash green 3 times
        for (int i = 0; i < 3; ++i) {
            setColor(2); // green
            renderFrame();
            this_thread::sleep_for(milliseconds(150));

            setColor(7); // default
            renderFrame();
            this_thread::sleep_for(milliseconds(150));
        }
    }
}

void SnakeGame::renderFrame() {
    vector<string> buffer(height, string(width, ' '));

    // Food
    if (food && food->pos.y >= 0 && food->pos.y < height && food->pos.x >= 0 && food->pos.x < width)
        buffer[food->pos.y][food->pos.x] = food->glyph();

    // Extra life
    if (extraLife && extraLife->y >= 0 && extraLife->y < height && extraLife->x >= 0 && extraLife->x < width)
        buffer[extraLife->y][extraLife->x] = '+';

    // Snake
    for (size_t i = 0; i < segments.size(); ++i) {
        auto s = segments[i];
        if (s->p.y >= 0 && s->p.y < height && s->p.x >= 0 && s->p.x < width)
            buffer[s->p.y][s->p.x] = (i == 0 ? '@' : 'x');
    }

    // Draw
    setCursor(0, 0);
    cout << "+" << string(width, '-') << "+\n";

    for (int y = 0; y < height; y++) {
        cout << "|" << buffer[y] << "|\n";
    }

    cout << "+" << string(width, '-') << "+\n";
    cout << "Score: " << score << "  Lives: " << lives << "  (Use Arrow Keys, Q to Quit)\n";
}
