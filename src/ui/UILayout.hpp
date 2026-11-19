#pragma once
#include <raylib.h>

// Clase que convierte coordenadas del lienzo de diseno (1280x720)
// al tamano real de la ventana.
class UILayout
{
public:
    // Recalcula escala y centrado con el tamano actual de la ventana.
    void Update();

    Vector2 Point(float x, float y) const;
    Rectangle Rect(float x, float y, float width, float height) const;

    float GetScale() const { return scale; }
    // Escala un tamano (fuente, borde, radio...).
    float ScaleValue(float value) const { return value * scale; }

private:
    float scale   = 1.0f;
    float offsetX = 0.0f;
    float offsetY = 0.0f;
};