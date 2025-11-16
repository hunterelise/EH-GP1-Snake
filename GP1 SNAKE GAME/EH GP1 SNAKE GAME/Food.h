#pragma once
#include "Entity.h"

// Food entity
class Food : public Entity {
public:
    Food() = default;
    Food(int x, int y) : Entity(x, y) {}

    // Character representation for display
    char glyph() const override { return 'O'; }
};
