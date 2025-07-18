#define CLAY_IMPLEMENTATION
#include "clay/clay.h"
#include "raylib.h"
#include "screen_manager.hpp"

auto main() -> int {
    ScreenManager::ScreenManager screen_manager{ ScreenManager::ScreenManager() };
    screen_manager.init();

    while (!screen_manager.m_should_quit_game) {
        screen_manager.update();

        BeginDrawing();
        ClearBackground(BLACK);

        screen_manager.render();

        EndDrawing();
    }

    ScreenManager::ScreenManager::shutdown();

    return 0;
}
