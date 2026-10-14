#include "Segment.hpp"


Segment::Segment(int width, height, Color color, Rectangle bounds, bool isDestructible = true): 
rectangle(bounds), width(width), height(height), color(color), isDestructible(isDestructible) {
}

void Segment::draw() const{
    DrawRectangleRec(bounds, color);
}
Rectangle Segment::getBounds() const{
    return bounds;
}
    bool Segment::checkCollision(Rectangle target) const {
        return CheckCollisionRecs(bounds, target);
    }

