#include <UILayout.hpp>
#include <algorithm>

void UILayout::Update()
{
    const float width  = static_cast<float>(GetScreenWidth());
    const float height = static_cast<float>(GetScreenHeight());

    scale   = std::min(width / 1280.0f, height / 720.0f);
    offsetX = (width  - 1280.0f * scale) * 0.5f;
    offsetY = (height -  720.0f * scale) * 0.5f;
}

Vector2 UILayout::Point(float x, float y) const
{
    return { offsetX + x * scale, offsetY + y * scale };
}

Rectangle UILayout::Rect(float x, float y, float width, float height) const
{
    return { offsetX + x * scale, offsetY + y * scale, width * scale, height * scale };
}