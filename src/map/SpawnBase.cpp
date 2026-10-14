#include "SpawnBase.hpp"

SpawnBase:SpawnBase(): base{} {
}

SpawnBase:SpawnBase(Rectangle base): base(base){
    
}


void SpawnBase:setBase(newBase){
    base=newBase;
}

Rectangle SpawnBase:getBase() const {
    return base;
}

Vector2 SpawnBase:getSpawnCoords() const {
    return {
        platform.x + platform.width / 2.0f,
        platform.y
    };
}

void SpawnBase::draw() const
{
    DrawRectangleRec(platform, GRAY);
}