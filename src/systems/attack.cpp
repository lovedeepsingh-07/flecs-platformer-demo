#include "components.hpp"
#include "systems.hpp"
#include "utils.hpp"
#include <box2d/box2d.h>
#include <stdexcept>

void AttackSystem::setup(GameContext::GameContext& ctx, b2WorldId world_id) {
    ctx.registry
        .observer<components::AttackEventComponent, components::AttackComponent>()
        .event(flecs::OnAdd)
        .each([&ctx, world_id](
                  flecs::entity curr_entity, const components::AttackEventComponent& attack_event,
                  components::AttackComponent& attack
              ) { attack.attacking = true; });
}

void AttackSystem::update(GameContext::GameContext& ctx, b2WorldId world_id) {
    ctx.registry
        .system<
            components::AttackComponent, components::PositionComponent, components::MovementComponent,
            components::TextureComponent, components::AnimationComponent,
            components::StateRegistryComponent, components::StateComponent>()
        .each([&ctx, world_id](
                  flecs::entity curr_entity, components::AttackComponent& attack,
                  const components::PositionComponent& pos,
                  const components::MovementComponent& movement,
                  const components::TextureComponent& texture,
                  const components::AnimationComponent& animation,
                  components::StateRegistryComponent& state_registry, components::StateComponent& state
              ) {
            // get current state registry
            auto state_registry_result =
                ctx.state_engine.get_state_registry(state_registry.state_registry_id);
            if (!state_registry_result) {
                throw std::runtime_error(state_registry_result.error().message);
            }
            const StateEngine::StateRegistry& curr_registry =
                state_registry_result->get();

            // get current state
            auto state_result = curr_registry.get_state(state.curr_state_id);
            if (!state_result) {
                throw std::runtime_error(state_result.error().message);
            }
            const StateEngine::State& curr_state = state_result->get();

            if (!attack.attacking) {
                return;
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

                if (cast_context.hit && attack.attacking && !attack.hit_some_entity
                    && !cast_context.hit_entity.has<components::AttackHitEventComponent>()
                    && curr_state.animation_data
                            .frames[animation.curr_frame_index]
                            ._type
                        == "active") {
                    attack.hit_some_entity = true;
                    cast_context.hit_entity.set(components::AttackHitEventComponent{
                        .direction = (texture.flipped ? -1 : 1) });
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

                    // TODO: there is some fucking issue here! that bug appears here
                    if (cast_context.hit && attack.attacking && !attack.hit_some_entity
                        && !cast_context.hit_entity.has<components::AttackHitEventComponent>()
                        && curr_state.animation_data
                                .frames[animation.curr_frame_index]
                                ._type
                            == "active") {
                        attack.hit_some_entity = true;
                        cast_context.hit_entity.set(components::AttackHitEventComponent{
                            .direction = (texture.flipped ? -1 : 1) });
                    }
                }
            }
        })
        .run();
}
