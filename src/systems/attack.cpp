#include "components.hpp"
#include "systems.hpp"
#include "utils.hpp"
#include <box2d/box2d.h>

void AttackSystem::update(GameContext::GameContext& ctx, b2WorldId world_id) {
    ctx.registry
        .system<components::AttackComponent, components::TextureComponent, components::MovementComponent, components::PositionComponent>()
        .each([&world_id](
                  flecs::entity curr_entity, components::AttackComponent& attack,
                  const components::TextureComponent& texture,
                  const components::MovementComponent& movement, components::PositionComponent& pos
              ) {
            if (curr_entity.has<components::AttackEventComponent>() && !attack.attacking) {
                attack.attacking = true;
            }
            if (curr_entity.has<components::PermanentRayCastComponent>()) {
                components::PermanentRayCastComponent raycast =
                    curr_entity.get<components::PermanentRayCastComponent>();
                if (texture.flipped) {
                    b2World_CastRay(
                        world_id,
                        (b2Vec2){ pos.x + raycast.start_offset.x,
                                  pos.y + raycast.start_offset.y },
                        (b2Vec2){ -raycast.translation.x, raycast.translation.y },
                        raycast.filter, Utils::cast_result_fcn, &raycast.user_data
                    );
                } else {
                    b2World_CastRay(
                        world_id,
                        (b2Vec2){ pos.x + raycast.start_offset.x,
                                  pos.y + raycast.start_offset.y },
                        (b2Vec2){ raycast.translation.x, raycast.translation.y },
                        raycast.filter, Utils::cast_result_fcn, &raycast.user_data
                    );
                }
            }
            if (curr_entity.has<components::PermanentRayCastListComponent>()) {
                components::PermanentRayCastListComponent raycast_list =
                    curr_entity.get<components::PermanentRayCastListComponent>();
                for (components::PermanentRayCastComponent& curr_raycast :
                     raycast_list.items) {
                    if (texture.flipped) {
                        b2World_CastRay(
                            world_id,
                            (b2Vec2){ pos.x + curr_raycast.start_offset.x,
                                      pos.y + curr_raycast.start_offset.y },
                            (b2Vec2){ -curr_raycast.translation.x,
                                      curr_raycast.translation.y },
                            curr_raycast.filter, Utils::cast_result_fcn,
                            &curr_raycast.user_data
                        );
                    } else {
                        b2World_CastRay(
                            world_id,
                            (b2Vec2){ pos.x + curr_raycast.start_offset.x,
                                      pos.y + curr_raycast.start_offset.y },
                            (b2Vec2){ curr_raycast.translation.x,
                                      curr_raycast.translation.y },
                            curr_raycast.filter, Utils::cast_result_fcn,
                            &curr_raycast.user_data
                        );
                    }
                }
            }
        })
        .run();
}
