#include "components.hpp"
#include "systems.hpp"
#include <raylib.h>

void ControllerSystem::update(GameContext::GameContext& ctx) {
    ctx.registry
        .system<components::MovementComponent, components::ControllerComponent>()
        .each([](flecs::entity curr_entity, components::MovementComponent& movement,
                 const components::ControllerComponent& controller) {
            if (IsKeyPressed(KEY_SPACE)
                && !curr_entity.has<components::JumpEventComponent>()
                && movement.on_ground) {
                curr_entity.add<components::JumpEventComponent>();
            }
            if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) {
                movement.left_idle_right = -1;
            }
            if (!IsKeyDown(KEY_A) && IsKeyDown(KEY_D)) {
                movement.left_idle_right = 1;
            }
            if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) {
                movement.left_idle_right = 0;
            }
        })
        .run();
}
