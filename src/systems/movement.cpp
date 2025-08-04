#include "components.hpp"
#include "constants.hpp"
#include "systems.hpp"
#include <box2d/box2d.h>

void MovementSystem::update(GameContext::GameContext& ctx) {
    ctx.registry
        .system<components::PhysicsComponent, components::MovementComponent>()
        .each([](flecs::entity curr_entity, components::PhysicsComponent& phy,
                 components::MovementComponent& movement) {
            b2Vec2 vel = b2Body_GetLinearVelocity(phy.body_id);

            if (curr_entity.has<components::JumpEventComponent>() && movement.on_ground) {
                vel.y = constants::PLAYER_JUMP_VEL;
                curr_entity.remove<components::JumpEventComponent>();
            }

            if (movement.left_idle_right == -1) {
                vel.x = -3.0F * constants::WORLD_SCALE * constants::FORCE_CONST;
            } else if (movement.left_idle_right == 1) {
                vel.x = 3.0F * constants::WORLD_SCALE * constants::FORCE_CONST;
            } else {
                vel.x = 0.0F;
            }

            if (!movement.on_ground) {
                if (vel.y < 0) {
                    movement.jumping = true;
                    movement.falling = false;
                }
                if (vel.y > 0) {
                    movement.jumping = false;
                    movement.falling = true;
                }
            } else {
                movement.jumping = false;
                movement.falling = false;
            }

            b2Body_SetLinearVelocity(phy.body_id, vel);
        })
        .run();
}
