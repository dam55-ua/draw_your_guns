#pragma once
#include <raylib.h>

// Fondo unico de todas las pantallas de menu.
inline void DrawMenuBackground()
{
    DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(),
                           Color{ 86, 178, 232, 255 },
                           Color{ 196, 235, 248, 255 });
}