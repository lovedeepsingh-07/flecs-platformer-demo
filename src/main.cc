#define CLAY_IMPLEMENTATION
#include "clay/clay.h"
#include "clay/renderers/raylib/clay_renderer_raylib.c"
#include "raylib.h"

int main() {
    Clay_Raylib_Initialize(0, 0, "game", FLAG_WINDOW_UNDECORATED);
    int screen_width = GetMonitorWidth(0);
    int screen_height = GetMonitorHeight(0);
    SetWindowSize(screen_width, screen_height);
    SetTargetFPS(75);

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);

        EndDrawing();
    }

    Clay_Raylib_Close();
}
