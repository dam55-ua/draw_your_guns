#include <MenuButton.hpp>
#include <UILayout.hpp>
#include <utility>

namespace
{
    const Color FILL       = { 245, 161, 28, 255 };
    const Color FILL_FOCUS = { 255, 188, 72, 255 };
    const Color BORDER     = {  46,  19,  5, 255 };
    const Color TEXT       = { 255, 244, 224, 255 };
}

MenuButton::MenuButton(std::string text, Rectangle designBounds)
    : label(std::move(text)), bounds(designBounds)
{
}

Rectangle MenuButton::GetScreenBounds(const UILayout& layout) const
{
    return layout.Rect(bounds.x, bounds.y, bounds.width, bounds.height);
}

bool MenuButton::Update(const UILayout& layout, bool hasFocus, bool activated)
{
    focused = hasFocus;

    const bool over = CheckCollisionPointRec(GetMousePosition(), GetScreenBounds(layout));
    const bool clicked = over && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    return clicked || activated;
}

void MenuButton::Draw(const UILayout& layout, const Font& font) const
{
    const Rectangle r = GetScreenBounds(layout);
    const float border = layout.ScaleValue(5.0f);

    DrawRectangleRounded({ r.x, r.y + layout.ScaleValue(5.0f), r.width, r.height },
                         0.3f, 8, Fade(BLACK, 0.35f));

    DrawRectangleRounded(r, 0.3f, 8, BORDER);

    const Rectangle inner = {
        r.x + border,
        r.y + border,
        r.width  - border * 2.0f,
        r.height - border * 2.0f
    };
    DrawRectangleRounded(inner, 0.25f, 8, focused ? FILL_FOCUS : FILL);

    const float size = layout.ScaleValue(30.0f);
    const float spacing = layout.ScaleValue(1.0f);
    const Vector2 textSize = MeasureTextEx(font, label.c_str(), size, spacing);

    DrawTextEx(font, label.c_str(),
               { r.x + (r.width  - textSize.x) * 0.5f,
                 r.y + (r.height - textSize.y) * 0.5f },
               size, spacing, TEXT);
}