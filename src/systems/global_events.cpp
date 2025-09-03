#include "constants.hpp"
#include "systems.hpp"

void systems::global_events(flecs::world& registry) {
    registry.system("Global Events")
        .kind(flecs::PostLoad)
        .run([](flecs::iter& iter) {
            flecs::world registry = iter.world();

            // handle quit events
            if (WindowShouldClose()) {
                registry.add<components::events::GameQuitEvent>();
            }

            // handle toggeling debug mode (only in game_scene)
            if (IsKeyPressed(constants::DEBUG_KEY)
                && registry.has<components::ActiveScene, components::scenes::Game>()) {
                if (!registry.has<components::global_options::DebugMode>()) {
                    registry.add<components::global_options::DebugMode>();
                } else {
                    registry.remove<components::global_options::DebugMode>();
                }
            }

            if (IsKeyPressed(KEY_ESCAPE)
                && registry.has<components::ActiveScene, components::scenes::Game>()) {
                if (registry.has<components::global_options::Paused>()) {
                    registry.remove<components::global_options::Paused>();
                } else {
                    registry.add<components::global_options::Paused>();
                }
            }
        })
        .add<components::system_types::Global>();
}
