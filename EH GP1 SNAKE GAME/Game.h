#pragma once

// Base class for games
class Game {
public:
    virtual ~Game() {}

    // Initialize game (implemented in derived classes)
    virtual void init() = 0;

    // Main game loop (implemented in derived classes)
    virtual void run() = 0;
};
