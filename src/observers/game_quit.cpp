#include "components.hpp"
#include "observers.hpp"

void observers::game_quit(flecs::world& registry) {
    registry.observer<components::events::GameQuitEvent>("GameQuitEvent on_add")
        .event(flecs::OnAdd)
        .run([](flecs::iter& iter) {
            flecs::world registry = iter.world();

            const auto& texture_engine = registry.get<components::Texture_Engine>();
            texture_engine.engine.unload_textures();

            registry.quit();
        });
}
