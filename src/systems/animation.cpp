#include "components.hpp"
#include "systems.hpp"

void AnimationSystem::update(GameContext::GameContext& ctx) {
    flecs::system texture_flipping_sys =
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
            });
    texture_flipping_sys.run();

    flecs::system animation_sys =
        ctx.registry
            .system<components::TextureComponent, components::AnimationComponent, components::StateRegistryComponent, components::StateComponent>()
            .each([&ctx](
                      flecs::entity curr_entity, components::TextureComponent& texture,
                      components::AnimationComponent& animation,
                      components::StateRegistryComponent& state_registry,
                      components::StateComponent& state
                  ) {
                StateEngine::StateRegistry& curr_registry =
                    ctx.state_engine.get_state_registry(state_registry.state_registry_id);
                StateEngine::State curr_state = curr_registry[state.curr_state_id];
                if (!animation.playing || animation.finished) {
                    return;
                }
                animation.time_accumulator += GetFrameTime();
                const Rectangle curr_frame =
                    curr_state.animation_data.frames[animation.curr_frame_index];
                if (animation.time_accumulator >= 0.1F) {
                    animation.time_accumulator -= 0.1F;
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
                    curr_state.animation_data.frames[animation.curr_frame_index];
            });
    animation_sys.run();
}
