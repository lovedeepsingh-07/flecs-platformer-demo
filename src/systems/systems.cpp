#include "systems.hpp"
#include "components.hpp"
#include "constants.hpp"

void systems::setup(flecs::world& registry) {
    registry.system("Global Events").kind(flecs::PostLoad).run([](flecs::iter& iter) {
        flecs::world registry = iter.world();
        // handle quit events
        if (WindowShouldClose()) {
            registry.quit();
        }

        // handle toggeling debug mode (only in game_scene)
        if (IsKeyPressed(constants::DEBUG_KEY)
            && registry.has<components::ActiveScene, components::Game_Scene>()) {
            if (!registry.has<components::DebugMode>()) {
                registry.add<components::DebugMode>();
            } else {
                registry.remove<components::DebugMode>();
            }
        }
    });

    systems::controller(registry);
    systems::movement(registry);
    systems::physics(registry);
    systems::camera(registry);
    systems::render(registry);
};
