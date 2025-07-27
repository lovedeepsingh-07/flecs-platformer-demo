#include "box2d/box2d.h"
#include "components.hpp"
#include "constants.hpp"
#include "systems.hpp"

void MovementSystem::update(GameContext& ctx) {
    flecs::system movement_sys =
        ctx.registry
            .system<components::PhysicsComponent, components::MovementComponent>()
            .each([](const components::PhysicsComponent& phy,
                     components::MovementComponent& movement) {
                b2Vec2 vel = b2Body_GetLinearVelocity(phy.body_id);

                if (movement.jump_requested && movement.on_ground) {
                    vel.y = constants::PLAYER_JUMP_VEL;
                }

                if (movement.left) {
                    vel.x = -3.0F * constants::WORLD_SCALE * constants::FORCE_CONST;
                } else if (movement.right) {
                    vel.x = 3.0F * constants::WORLD_SCALE * constants::FORCE_CONST;
                } else {
                    vel.x = 0.0F;
                }

                if (vel.y > 0) {
                    movement.jumping = false;
                    movement.falling = true;
                } else if (vel.y < 0) {
                    movement.jumping = true;
                    movement.falling = false;
                } else {
                    movement.jumping = false;
                    movement.falling = false;
                }

                movement.jump_requested = false;
                b2Body_SetLinearVelocity(phy.body_id, vel);
            });
    movement_sys.run();
}
