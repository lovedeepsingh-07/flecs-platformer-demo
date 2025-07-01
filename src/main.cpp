#define CLAY_IMPLEMENTATION
#include "clay/clay.h"
#include "clay/renderers/raylib/clay_renderer_raylib.c"
#include "physics/physics.hpp"
#include "raylib.h"

auto main() -> int {
    Clay_Raylib_Initialize(0, 0, "game", FLAG_WINDOW_UNDECORATED);
    int screen_width = GetMonitorWidth(0);
    int screen_height = GetMonitorHeight(0);
    SetWindowSize(screen_width, screen_height);
    SetTargetFPS(75);

    Physics::World world;
    world.world_gravity = Vector2(0, 980);
    Physics::Object player{
        .position = { (float)screen_width / 2, 0 },
        .dimensions = { 32, 32 },
        .velocity = { 0, 0 },
        .force = { 0, 0 },
        .mass = 5,
    };
    world.add_object(&player);

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();
        world.step(delta_time);

        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle((int)player.position.x, (int)player.position.y, 32, 32, RAYWHITE);

        if (IsKeyPressed(KEY_SPACE)) {
            player.velocity.y = -500;
        }

        EndDrawing();
    }

    world.remove_object(&player);

    Clay_Raylib_Close();

    return 0;
}
