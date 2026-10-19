#pragma once
#include <raylib.h>

class SpawnBase
{
private:
    Rectangle base;

public:
    SpawnBase();
    SpawnBase(Rectangle base);

    void setBase(Rectangle newBase);
    Rectangle getBase() const;
    Vector2 getSpawnCoords() const;
    void draw() const;
};