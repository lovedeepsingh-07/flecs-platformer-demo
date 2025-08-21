#include "components.hpp"
#include "systems.hpp"

void systems::state(flecs::world& registry) {
    registry
        .system<components::State, components::Movement, components::Animation>()
        .kind(flecs::PreUpdate)
        .each([](flecs::entity curr_entity, components::State& state,
                 const components::Movement& movement, components::Animation& animation) {
            flecs::world registry = curr_entity.world();
            const auto& state_engine = registry.get<components::State_Engine>();

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

            std::string next_state_id = state.curr_state_id;

            if (!movement.on_ground) {
                if (curr_state.can_transition_to("jump")) {
                    next_state_id = "jump";
                }
            } else if (movement.left_idle_right == 0) {
                if (curr_state.can_transition_to("idle")) {
                    next_state_id = "idle";
                }
            } else {
                if (curr_state.can_transition_to("walk")) {
                    next_state_id = "walk";
                }
            }

            if (next_state_id != state.curr_state_id) {
                auto next_state_result = curr_registry.get_state(next_state_id);
                if (!next_state_result) {
                    throw std::runtime_error(next_state_result.error().message);
                }
                const StateEngine::State& next_state = next_state_result->get();

                auto state_transition_result = curr_state.get_transition(next_state_id);
                if (!state_transition_result) {
                    throw std::runtime_error(state_transition_result.error().message);
                }
                const StateEngine::State_can_transition_to& curr_transition =
                    state_transition_result->get();

                int next_state_frame = curr_transition.frame;
                if (next_state_frame == -1) {
                    animation.curr_frame_index =
                        (int)next_state.animation_data.frames.size() - 1;
                } else {
                    animation.curr_frame_index = next_state_frame;
                }

                state.curr_state_id = next_state_id;
                animation.curr_frame_index = 0;
                animation.time_accumulator = 0.0F;
                animation.playing = true;
            }
        })
        .run();
}
