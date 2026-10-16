#include "Segment.hpp"


Segment::Segment(int width, int height, Color color, Rectangle bounds, bool isDestructible): 
bounds(bounds), color(color){
    this->width = width;
    this->height = height;
    this->isDestructible = isDestructible;
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

