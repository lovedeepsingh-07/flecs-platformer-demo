#include "components.hpp"
#include "constants.hpp"
#include "systems.hpp"

void AnimationSystem::update(GameContext::GameContext& ctx) {
    ctx.registry
        .system<components::TextureComponent, components::MovementComponent>()
        .each([](flecs::entity curr_entity, components::TextureComponent& texture,
                 components::MovementComponent& movement) {
            if (movement.left_idle_right == -1) {
                texture.flipped = true;
            }
            if (movement.left_idle_right == 1) {
                texture.flipped = false;
            }
        })
        .run();

    ctx.registry
        .system<
            components::TextureComponent, components::AnimationComponent, components::StateRegistryComponent,
            components::StateComponent, components::AttackComponent>()
        .each([&ctx](
                  flecs::entity curr_entity, components::TextureComponent& texture,
                  components::AnimationComponent& animation,
                  components::StateRegistryComponent& state_registry,
                  components::StateComponent& state, components::AttackComponent& attack
              ) {
            // get current state registry
            StateEngine::StateRegistry curr_registry;
            auto state_registry_result =
                ctx.state_engine.get_state_registry(state_registry.state_registry_id);
            if (!state_registry_result) {
                throw std::runtime_error(state_registry_result.error().message);
            }
            curr_registry = *state_registry_result;

            // get current state
            StateEngine::State curr_state;
            auto state_result = curr_registry.get_state(state.curr_state_id);
            if (!state_result) {
                throw std::runtime_error(state_result.error().message);
            }
            curr_state = *state_result;

            if (!animation.playing || animation.finished) {
                if (attack.attacking
                    && curr_entity.has<components::AttackEventComponent>()) {
                    curr_entity.remove<components::AttackEventComponent>();
                    attack.attacking = false;
                    attack.hit_some_entity = false;
                }
                return;
            }

            animation.time_accumulator += GetFrameTime();
            const StateEngine::AnimationFrame curr_frame =
                curr_state.animation_data.frames[animation.curr_frame_index];

            if (animation.time_accumulator >= constants::ANIMATION_FRAME_TIME) {
                animation.time_accumulator -= constants::ANIMATION_FRAME_TIME;
                animation.curr_frame_index++;

                if (animation.curr_frame_index
                    >= curr_state.animation_data.frames.size()) {
                    if (curr_state.animation_data.loop) {
                        animation.curr_frame_index = 0;
                    } else {
                        animation.curr_frame_index =
                            (int)curr_state.animation_data.frames.size() - 1;
                        animation.finished = true;
                        animation.playing = false;
                    }
                }
            }

            // Update texture rect
            texture.texture =
                ctx.texture_engine.get_texture(curr_state.animation_data.texture_id);
            texture.source_rect =
                curr_state.animation_data.frames[animation.curr_frame_index].source_rect;
        })
        .run();
}
