#include "constants.hpp"
#include "systems.hpp"
#include <algorithm>

void systems::movement(flecs::world& registry) {
    registry
        .system<components::events::BufferedDashEvent>("Dash Buffering System")
        .kind(flecs::PreUpdate)
        .each([](flecs::entity curr_entity, components::events::BufferedDashEvent& buffered_dash_event) {
            buffered_dash_event.buffer_time -= curr_entity.world().delta_time();
            if (buffered_dash_event.buffer_time <= 0) {
                curr_entity.remove<components::events::BufferedDashEvent>();
            }
        })
        .add<components::system_types::Update>();

    registry.system<components::events::DashEvent>("Dash System")
        .kind(flecs::PreUpdate)
        .each([](flecs::entity curr_entity, components::events::DashEvent& dash_event) {
            dash_event.dash_time -= curr_entity.world().delta_time();
            if (dash_event.dash_time <= 0) {
                curr_entity.destruct();
            }
        })
        .add<components::system_types::Update>();

    registry
        .system<components::events::BufferedJumpEvent>("Jump Buffering System")
        .kind(flecs::PreUpdate)
        .each([](flecs::entity curr_entity, components::events::BufferedJumpEvent& buffered_jump_event) {
            buffered_jump_event.buffer_time -= curr_entity.world().delta_time();
            if (buffered_jump_event.buffer_time <= 0) {
                curr_entity.remove<components::events::BufferedJumpEvent>();
            }
        })
        .add<components::system_types::Update>();

    registry
        .system<components::TextureComponent, components::Movement>(
            "Handle Texture Flipping"
        )
        .kind(flecs::PreUpdate)
        .each([](flecs::entity curr_entity, components::TextureComponent& texture,
                 const components::Movement& movement) {
            if (movement.left_idle_right == -1) {
                texture.flipped = true;
            }
            if (movement.left_idle_right == 1) {
                texture.flipped = false;
            }
        })
        .add<components::system_types::Update>();

    registry
        .system<components::Movement, components::PhysicalBody>("Apply Movement")
        .kind(flecs::PreUpdate)
        .each([](flecs::entity curr_entity, const components::Movement& movement,
                 const components::PhysicalBody& body) {
            b2Vec2 vel = b2Body_GetLinearVelocity(body.body_id);

            if (curr_entity.target<components::Attack_Entity>().is_valid()
                && movement.on_ground) {
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

            b2Body_SetLinearVelocity(body.body_id, vel);
        })
        .add<components::system_types::Update>();
};
