#include "components.hpp"
#include "raylib.h"
#include "systems.hpp"

void ControllerSystem::update(GameContext& ctx) {
    flecs::system controller_sys =
        ctx.registry
            .system<components::ControllerComponent, components::MovementComponent>()
            .each([](const components::ControllerComponent& controller,
                     components::MovementComponent& movement) {
                if (!movement.right) {
                    movement.left = IsKeyDown(KEY_A);
                }
                if (!movement.left) {
                    movement.right = IsKeyDown(KEY_D);
                }
                if (!movement.jump_requested) {
                    movement.jump_requested = IsKeyPressed(KEY_SPACE);
                }
            });
    controller_sys.run();
}
