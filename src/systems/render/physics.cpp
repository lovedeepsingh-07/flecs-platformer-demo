#include "components.hpp"
#include "systems.hpp"

void systems::render::physics(flecs::world& registry) {
    registry.system("Render Physical Hitboxes").kind(flecs::PreStore).run([](flecs::iter& iter) {
        flecs::world registry = iter.world();
        if (!registry.has<components::DebugMode>()
            || !registry.has<components::PhysicalWorld>()
            || !registry.has<components::ActiveScene, components::Game_Scene>()
            || !registry.has<components::PhysicalDebugDraw>()) {
            return;
        }

        auto physical_world = registry.get<components::PhysicalWorld>();
        auto debug_draw = registry.get<components::PhysicalDebugDraw>();
        b2World_Draw(physical_world.world_id, &debug_draw.debug_draw);
    });
}
