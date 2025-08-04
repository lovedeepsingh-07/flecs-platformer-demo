#include "components.hpp"
#include "systems.hpp"
#include <raylib.h>

void ControllerSystem::update(GameContext::GameContext& ctx) {
    ctx.registry
        .system<components::MovementComponent, components::AttackComponent, components::ControllerComponent>()
        .each([](flecs::entity curr_entity, components::MovementComponent& movement,
                 components::AttackComponent& attack,
                 const components::ControllerComponent& controller) {
            if (IsKeyPressed(KEY_SPACE)
                && !curr_entity.has<components::JumpEventComponent>()
                && movement.on_ground) {
                if (curr_entity.has<components::AttackEventComponent>()
                    && attack.attacking) {
                    curr_entity.remove<components::AttackEventComponent>();
                    attack.attacking = false;
                }
                curr_entity.add<components::JumpEventComponent>();
            }
            if (IsKeyPressed(KEY_J) && !curr_entity.has<components::AttackEventComponent>()
                && !attack.attacking) {
                curr_entity.add<components::AttackEventComponent>();
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
