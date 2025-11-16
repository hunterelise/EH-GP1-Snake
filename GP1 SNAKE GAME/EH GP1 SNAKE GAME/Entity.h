#pragma once
#include "Point.h"  // 2D coordinates (x, y)

// Base class for all game entities
class Entity {
public:
    Point pos;  // Entity position

    Entity() = default;
    Entity(int x, int y) : pos{ x, y } {}
    virtual ~Entity() {}

    // Update entity state (override in derived classes)
    virtual void update() {}

    // Character representation for display
    virtual char glyph() const { return '?'; }
};
