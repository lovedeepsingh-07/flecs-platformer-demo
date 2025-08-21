#include "components.hpp"
#include "constants.hpp"
#include "systems.hpp"

void systems::animation(flecs::world& registry) {
    registry
        .system<components::Animation, components::State, components::TextureComponent>("Play Animations")
        .kind(flecs::PreUpdate)
        .each([](flecs::entity curr_entity, components::Animation& animation,
                 const components::State& state, components::TextureComponent& texture) {
            flecs::world registry = curr_entity.world();
            const auto& state_engine = registry.get<components::State_Engine>();
            const auto& texture_engine = registry.get<components::Texture_Engine>();

            // get current state registry
            auto state_registry_result =
                state_engine.engine.get_state_registry(state.state_registry_id);
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

            animation.time_accumulator += GetFrameTime();
            const StateEngine::AnimationFrame& curr_frame =
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
                        animation.playing = false;
                    }
                }
            }

            // Update texture rect
            texture.texture =
                texture_engine.engine.get_texture(curr_state.animation_data.texture_id);
            texture.source_rect =
                curr_state.animation_data.frames[animation.curr_frame_index].source_rect;
        });
}
