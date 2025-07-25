#include "components.hpp"
#include "raylib.h"
#include "systems.hpp"

void ControllerSystem::update(GameContext& ctx) {
    flecs::system controller_sys =
        ctx.registry
            .system<components::ControllerComponent, components::MovementComponent>()
            .each([](const components::ControllerComponent& controller,
                     components::MovementComponent& movement) {
                movement.left = IsKeyDown(KEY_A);
                movement.right = IsKeyDown(KEY_D);
                if (!movement.jump_requested) {
                    movement.jump_requested = IsKeyPressed(KEY_SPACE);
                }
            });
    controller_sys.run();
}
