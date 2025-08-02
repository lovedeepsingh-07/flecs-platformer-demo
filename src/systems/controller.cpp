#include "components.hpp"
#include "systems.hpp"
#include <raylib.h>

void ControllerSystem::update(GameContext& ctx) {
    flecs::query<> controller_query = ctx.registry.query_builder()
                                          .with<components::ControllerComponent>()
                                          .with<components::MovementComponent>()
                                          .build();
    ctx.registry.defer_begin();
    controller_query.run([](flecs::iter& iter) {
        while (iter.next()) {
            auto movement = iter.field<components::MovementComponent>(1);
            for (auto i : iter) {
                flecs::entity curr_entity = iter.entity(i);
                if (IsKeyPressed(KEY_SPACE)
                    && !curr_entity.has<components::JumpEventComponent>()
                    && movement[i].on_ground) {
                    curr_entity.add<components::JumpEventComponent>();
                }
                if (IsKeyPressed(KEY_J)
                    && !curr_entity.has<components::AttackEventComponent>()
                    && movement[i].on_ground) {
                    curr_entity.add<components::AttackEventComponent>();
                }
                if (!movement[i].right) {
                    movement[i].left = IsKeyDown(KEY_A);
                }
                if (!movement[i].left) {
                    movement[i].right = IsKeyDown(KEY_D);
                }
            }
        }
    });
    ctx.registry.defer_end();
}
