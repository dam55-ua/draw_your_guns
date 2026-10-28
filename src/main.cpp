#include <StateMachine.hpp>
#include <MainGameState.hpp>
#include <memory>

extern "C" {
    #include <raylib.h>
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Draw your guns!");
    ToggleFullscreen();
    SetTargetFPS(60);

    float delta_time = 0.0f;

    {
        StateMachine state_machine;

        state_machine.add_state(std::make_unique<MainGameState>(), false);

        state_machine.handle_state_changes(delta_time);

        while (!WindowShouldClose() &&
               !state_machine.is_game_ending())
        {
            delta_time = GetFrameTime();

            state_machine.handle_state_changes(delta_time);
            state_machine.getCurrentState()->handleInput();
            state_machine.getCurrentState()->update(delta_time);
            state_machine.getCurrentState()->render();
        }

    } // StateMachine y sus estados se destruyen aquí.

    CloseWindow();

    return 0;
}