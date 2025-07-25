#define CLAY_IMPLEMENTATION
#include "clay/clay.h"
#include "raylib.h"
#include "scene_manager.hpp"

int main() {
    bool should_quit_game = false;
    GameContext ctx{ .registry = flecs::world{},
                     .event_system = EventSystem::EventSystem{} };


    SceneManager::SceneManager scene_manager{ SceneManager::SceneManager() };
    scene_manager.init();
    scene_manager.add_scene(ctx, SceneLabel::MainMenu, std::make_shared<MainMenuScene>());
    scene_manager.add_scene(ctx, SceneLabel::Game, std::make_shared<GameScene>());
    scene_manager.switch_to(ctx, SceneLabel::MainMenu);

    // global event handlers
    ctx.event_system.on<EventSystem::EventType::WindowQuitEvent>(
        [&should_quit_game](const EventSystem::WindowQuitEvent& event) {
            should_quit_game = true;
        }
    );
    ctx.event_system.on<EventSystem::EventType::SceneSwitchEvent>(
        [&ctx, &scene_manager](const EventSystem::SceneSwitchEvent& event) {
            scene_manager.switch_to(ctx, event.to);
        }
    );

    while (!should_quit_game) {
        scene_manager.update(ctx);

        BeginDrawing();
        ClearBackground(BLACK);

        scene_manager.render(ctx);

        EndDrawing();
    }

    scene_manager.shutdown();

    return 0;
}
