#pragma once
#include <raylib.h>
#include <string>

class UILayout;

class MenuButton
{
public:
    MenuButton() = default;
    MenuButton(std::string text, Rectangle designBounds);

    // Devuelve true el frame en el que se pulsa (raton o teclado).
    bool Update(const UILayout& layout, bool focused, bool activated);
    void Draw(const UILayout& layout, const Font& font) const;

    Rectangle GetScreenBounds(const UILayout& layout) const;

private:
    std::string label;
    Rectangle bounds = { 0, 0, 0, 0 };   // coordenadas de diseno
    bool focused = false;
};