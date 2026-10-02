#include <StateMachine.hpp>
#include <MainGameState.hpp>
#include <memory>
#include <chrono>

extern "C" {
    #include <raylib.h>
}

int main()
{ 
    // El modo de pantalla completa se configura antes de crear la ventana.
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Draw your guns!");
    SetTargetFPS(60);

    float delta_time = 0.0f;

    StateMachine state_machine = StateMachine();
    state_machine.add_state(std::make_unique<MainGameState>(), false);
    state_machine.handle_state_changes(delta_time);

    while (!state_machine.is_game_ending())
    {
        if (WindowShouldClose())
        {
            break;
        }

        // Se mide el tiempo entre frames para que la actualización no dependa de los FPS.
        delta_time = GetFrameTime();
        state_machine.handle_state_changes(delta_time);
        state_machine.getCurrentState()->handleInput();
        state_machine.getCurrentState()->update(delta_time);
        state_machine.getCurrentState()->render();       
    }

    CloseWindow();
    return 0;
}