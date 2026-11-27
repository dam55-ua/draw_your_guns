#pragma once
#include <GameState.hpp>
#include <MenuButton.hpp>
#include <UILayout.hpp>
#include <vector>

class MainMenuState : public GameState
{
    public:
        MainMenuState();
        ~MainMenuState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause() override {}
        void resume() override {}

    private:
        void LoadResources();
        void DrawLogo() const;

        Texture2D logo = {};
        Font font = {};

        UILayout layout;
        std::vector<MenuButton> buttons;
        int focusIndex = 0;
        bool activateFocused = false;
};