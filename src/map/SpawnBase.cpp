#include "SpawnBase.hpp"

SpawnBase::SpawnBase(): base{} {
}

SpawnBase::SpawnBase(Rectangle base): base(base){
    
}


void SpawnBase::setBase(Rectangle newBase){
    base=newBase;
}

Rectangle SpawnBase::getBase() const {
    return base;
}

Vector2 SpawnBase::getSpawnCoords() const {
    return {
        base.x + base.width / 2.0f,
        base.y
    };
}

void SpawnBase::draw() const
{
    DrawRectangleRec(base, GRAY);
}