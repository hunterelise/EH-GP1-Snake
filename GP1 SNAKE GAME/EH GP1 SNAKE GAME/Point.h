#pragma once

// 2D coordinate
struct Point {
    int x{ 0 };
    int y{ 0 };

    Point() = default;
    Point(int x_, int y_) : x(x_), y(y_) {}

    // Compare points
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};
