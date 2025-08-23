#include "components.hpp"
#include "constants.hpp"
#include "systems.hpp"

// NOTE: position component represents the center point of any entity in my game
// but in raylib, the position is represented by the top-left corner for rendering
// so while rendering we have to convert my game's center point position to raylib's
// top-left point position by using the width and height of the entity
void systems::render::setup(flecs::world& registry) {
    registry.system("Prepare Frame").kind(flecs::PostUpdate).run([](flecs::iter& iter) {
        flecs::world registry = iter.world();

        BeginDrawing();
        ClearBackground(constants::BACKGROUND_COLOR);

        // only for "Game_Scene" when registry has a "GlobalCamera"
        if (!registry.has<components::ActiveScene, components::Game_Scene>()
            || !registry.has<components::GlobalCamera>()) {
            return;
        }
        auto global_camera = registry.get<components::GlobalCamera>();
        BeginMode2D(global_camera.camera);
    });

    systems::render::texture(registry);
    systems::render::rectangle(registry);
    systems::render::physics(registry);
    systems::render::GUI(registry);

    registry.system("Finish Frame").kind(flecs::OnStore).run([](flecs::iter& iter) {
        EndDrawing();
    });
};
