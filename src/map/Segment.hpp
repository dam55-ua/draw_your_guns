#pragma once
#include <raylib.h>

class Segment
{
private:
    int height;
    int width;
    Color color;
    Rectangle bounds;
    bool isDestructible;

public:
    Segment(int width, int height, Color color, Rectangle bounds, bool isDestructible = true);

    void draw() const;
    Rectangle getBounds() const;
    bool checkCollision(Rectangle target) const;
};