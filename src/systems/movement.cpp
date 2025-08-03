#include "components.hpp"
#include "constants.hpp"
#include "systems.hpp"
#include <box2d/box2d.h>

void MovementSystem::update(GameContext::GameContext& ctx) {
    flecs::query<> movement_query = ctx.registry.query_builder()
                                        .with<components::PhysicsComponent>()
                                        .with<components::MovementComponent>()
                                        .build();
    ctx.registry.defer_begin();
    movement_query.run([](flecs::iter& iter) {
        while (iter.next()) {
            auto phy = iter.field<components::PhysicsComponent>(0);
            auto movement = iter.field<components::MovementComponent>(1);

            for (auto i : iter) {
                flecs::entity curr_entity = iter.entity(i);
                b2Vec2 vel = b2Body_GetLinearVelocity(phy[i].body_id);

                if (curr_entity.has<components::JumpEventComponent>()
                    && movement[i].on_ground) {
                    vel.y = constants::PLAYER_JUMP_VEL;
                    curr_entity.remove<components::JumpEventComponent>();
                }

                if (movement[i].left) {
                    vel.x = -3.0F * constants::WORLD_SCALE * constants::FORCE_CONST;
                } else if (movement[i].right) {
                    vel.x = 3.0F * constants::WORLD_SCALE * constants::FORCE_CONST;
                } else {
                    vel.x = 0.0F;
                }

                if (!movement[i].on_ground) {
                    if (vel.y < 0) {
                        movement[i].jumping = true;
                        movement[i].falling = false;
                    }
                    if (vel.y > 0) {
                        movement[i].jumping = false;
                        movement[i].falling = true;
                    }
                } else {
                    movement[i].jumping = false;
                    movement[i].falling = false;
                }

                b2Body_SetLinearVelocity(phy[i].body_id, vel);
            }
        }
    });
    ctx.registry.defer_end();
}
