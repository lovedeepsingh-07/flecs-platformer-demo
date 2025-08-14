#include "components.hpp"
#include "constants.hpp"
#include "systems.hpp"
#include <algorithm>
#include <box2d/box2d.h>

void MovementSystem::update(GameContext::GameContext& ctx) {
    ctx.registry
        .system<components::PhysicsComponent, components::MovementComponent, components::AttackComponent>()
        .each([](flecs::entity curr_entity, components::PhysicsComponent& phy,
                 components::MovementComponent& movement, components::AttackComponent& attack) {
            b2Vec2 vel = b2Body_GetLinearVelocity(phy.body_id);

            if (curr_entity.has<components::JumpEventComponent>() && movement.on_ground) {
                vel.y = constants::PLAYER_JUMP_VEL;
                curr_entity.remove<components::JumpEventComponent>();
            }

            if (attack.attacking && movement.on_ground) {
                vel.x = 0.0F;
            } else {
                float target_speed = 0.0F;
                float delta_time = GetFrameTime();

                // figure out what the max speed will be along with the direction
                if (movement.left_idle_right == -1) {
                    target_speed = -constants::MAX_PLAYER_RUN_VEL;
                } else if (movement.left_idle_right == 1) {
                    target_speed = constants::MAX_PLAYER_RUN_VEL;
                }

                // figure out the accel/decel rate based on whether is player is in air or not
                float accel_rate = movement.on_ground ? constants::ACCEL_GROUND
                                                      : constants::ACCEL_AIR;

                float decel_rate = movement.on_ground ? constants::DECEL_GROUND
                                                      : constants::DECEL_AIR;

                // calcuate the difference betwee the current velocity of the player and the target velocity
                float vel_diff = target_speed - vel.x;

				// calculate how much we have to change the velocity to smoothly transition between velocities
                float vel_change = 0.0F;
                if (target_speed != 0.0F) {
                    vel_change =
                        std::clamp(vel_diff, -accel_rate * delta_time, accel_rate * delta_time);
                } else {
                    vel_change =
                        std::clamp(-vel.x, -decel_rate * delta_time, decel_rate * delta_time);
                }
                vel.x += vel_change;
            }

            if (!movement.on_ground) {
                movement.jumping = vel.y < 0;
                movement.falling = vel.y > 0;
            } else {
                movement.jumping = false;
                movement.falling = false;
            }

            b2Body_SetLinearVelocity(phy.body_id, vel);
        })
        .run();
}
