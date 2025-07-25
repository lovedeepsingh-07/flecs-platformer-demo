#define CLAY_IMPLEMENTATION
#include "clay/clay.h"
#include "raylib.h"
#include "scene_manager.hpp"

int main() {
    bool should_quit_game = false;
    GameContext ctx{ .registry = flecs::world{},
                     .event_system = EventEngine::EventEngine{} };


    SceneManager scene_manager{ SceneManager() };
    scene_manager.init();
    scene_manager.add_scene(ctx, 0, std::make_shared<MainMenuScene>());
    scene_manager.add_scene(ctx, 1, std::make_shared<GameScene>());
    scene_manager.switch_to(ctx, 0);

    // global event handlers
    ctx.event_system.on<EventEngine::EventType::WindowQuitEvent>(
        [&should_quit_game](const EventEngine::WindowQuitEvent& event) {
            should_quit_game = true;
        }
    );
    ctx.event_system.on<EventEngine::EventType::SceneSwitchEvent>(
        [&ctx, &scene_manager](const EventEngine::SceneSwitchEvent& event) {
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
