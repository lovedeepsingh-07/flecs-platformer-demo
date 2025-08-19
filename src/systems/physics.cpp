#include "components.hpp"
#include "constants.hpp"
#include "systems.hpp"

void systems::physics(flecs::world& registry) {
    registry.system("Step Physical World").kind(flecs::PreUpdate).run([](flecs::iter& iter) {
        flecs::world registry = iter.world();

        if (!registry.has<components::PhysicalWorld>()
            || !registry.has<components::ActiveScene, components::Game_Scene>()) {
            return;
        }
        auto physical_world = registry.get<components::PhysicalWorld>();

        b2World_Step(physical_world.world_id, constants::TIME_STEP, constants::SUB_STEP_COUNT);
    });

    registry
        .system<components::PhysicalBody, components::Position>(
            "Apply Physical Calculations"
        )
        .kind(flecs::PreUpdate)
        .each([](const components::PhysicalBody& body, components::Position& pos) {
            b2Vec2 body_pos = b2Body_GetPosition(body.body_id);
            pos.x = body_pos.x;
            pos.y = body_pos.y;
        });
};
