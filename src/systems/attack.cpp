#include "components.hpp"
#include "systems.hpp"
#include "utils.hpp"
#include <box2d/box2d.h>
#include <iostream>

void AttackSystem::setup(GameContext::GameContext& ctx, b2WorldId world_id) {
    ctx.registry.observer<components::PositionComponent>()
        .event<components::AttackHitEventComponent>()
        .each([](const components::PositionComponent& pos) {
            std::cout << "(" << pos.x << "," << pos.y << ")" << '\n';
        });
    ctx.registry.observer<components::AttackEventComponent>()
        .with<components::AttackComponent>()
        .with<components::TextureComponent>()
        .filter()
        .with<components::MovementComponent>()
        .filter()
        .with<components::PositionComponent>()
        .filter()
        .event(flecs::OnAdd)
        .each([](flecs::entity curr_entity, const components::AttackEventComponent& attack_event) {
            auto attack = curr_entity.get<components::AttackComponent>();
            auto texture = curr_entity.get<components::TextureComponent>();
            auto movement = curr_entity.get<components::MovementComponent>();
            auto pos = curr_entity.get<components::PositionComponent>();
            std::cout << "shit works!" << '\n';
        });
}

void AttackSystem::update(GameContext::GameContext& ctx, b2WorldId world_id) {
    ctx.registry
        .system<components::AttackComponent, components::TextureComponent, components::MovementComponent, components::PositionComponent>()
        .each([&ctx, &world_id](
                  flecs::entity curr_entity, components::AttackComponent& attack,
                  const components::TextureComponent& texture,
                  const components::MovementComponent& movement, components::PositionComponent& pos
              ) {
            if (curr_entity.has<components::AttackEventComponent>() && !attack.attacking) {
                attack.attacking = true;
            }


            // singular raycast
            if (curr_entity.has<components::PermanentRayCastComponent>()) {
                components::PermanentRayCastComponent raycast =
                    curr_entity.get<components::PermanentRayCastComponent>();

                Utils::CastContext cast_context{};
                cast_context.cast_user_data = &raycast.user_data;

                if (texture.flipped) {
                    b2World_CastRay(
                        world_id,
                        (b2Vec2){ pos.x + raycast.start_offset.x,
                                  pos.y + raycast.start_offset.y },
                        (b2Vec2){ -raycast.translation.x, raycast.translation.y },
                        raycast.filter, Utils::cast_result_fcn, &cast_context
                    );
                } else {
                    b2World_CastRay(
                        world_id,
                        (b2Vec2){ pos.x + raycast.start_offset.x,
                                  pos.y + raycast.start_offset.y },
                        (b2Vec2){ raycast.translation.x, raycast.translation.y },
                        raycast.filter, Utils::cast_result_fcn, &cast_context
                    );
                }

                if (cast_context.hit && attack.attacking) {
                    ctx.registry.event<components::AttackHitEventComponent>()
                        .id<components::PositionComponent>()
                        .entity(cast_context.hit_entity)
                        .emit();
                }
            }

            // raycast list
            if (curr_entity.has<components::PermanentRayCastListComponent>()) {
                components::PermanentRayCastListComponent raycast_list =
                    curr_entity.get<components::PermanentRayCastListComponent>();
                for (components::PermanentRayCastComponent& curr_raycast :
                     raycast_list.items) {

                    Utils::CastContext cast_context{};
                    cast_context.cast_user_data = &curr_raycast.user_data;

                    if (texture.flipped) {
                        b2World_CastRay(
                            world_id,
                            (b2Vec2){ pos.x + curr_raycast.start_offset.x,
                                      pos.y + curr_raycast.start_offset.y },
                            (b2Vec2){ -curr_raycast.translation.x,
                                      curr_raycast.translation.y },
                            curr_raycast.filter, Utils::cast_result_fcn, &cast_context
                        );
                    } else {
                        b2World_CastRay(
                            world_id,
                            (b2Vec2){ pos.x + curr_raycast.start_offset.x,
                                      pos.y + curr_raycast.start_offset.y },
                            (b2Vec2){ curr_raycast.translation.x,
                                      curr_raycast.translation.y },
                            curr_raycast.filter, Utils::cast_result_fcn, &cast_context
                        );
                    }

                    if (cast_context.hit && attack.attacking) {
                        ctx.registry
                            .event<components::AttackHitEventComponent>()
                            .id<components::PositionComponent>()
                            .entity(cast_context.hit_entity)
                            .emit();
                    }
                }
            }
        })
        .run();
}
