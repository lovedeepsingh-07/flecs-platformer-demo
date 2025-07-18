#include "systems/controller.hpp"
#include "box2d/box2d.h"
#include "components.hpp"
#include "constants.hpp"
#include "raylib.h"

void ControllerSystem::update(flecs::world& registry) {
    flecs::system controller_sys =
        registry
            .system<components::PhysicsComponent, components::ControllerComponent>("controller")
            .each([](const components::PhysicsComponent& phy,
                     const components::ControllerComponent& controller) {
                b2Vec2 vel = b2Body_GetLinearVelocity(phy.body_id);

                if (IsKeyPressed(KEY_SPACE)) {
                    vel.y = -5.0F * constants::WORLD_SCALE * constants::FORCE_CONST;
                }

                if (IsKeyDown(KEY_A)) {
                    vel.x = -3.0F * constants::WORLD_SCALE * constants::FORCE_CONST;
                } else if (IsKeyDown(KEY_D)) {
                    vel.x = 3.0F * constants::WORLD_SCALE * constants::FORCE_CONST;
                } else {
                    vel.x = 0.0F;
                }

                b2Body_SetLinearVelocity(phy.body_id, vel);
            });
    controller_sys.run();
}
