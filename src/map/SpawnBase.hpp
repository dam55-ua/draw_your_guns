#pragma once
#include <raylib.h>

class SpawnBase
{
private:
    Rectangle platform;

public:
    SpawnBase();
    SpawnBase(Rectangle platform);

    void setPlatform(Rectangle newPlatform);
    Rectangle getPlatform() const;
    Vector2 getSpawnPosition() const;
    void draw() const;
};