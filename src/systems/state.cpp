#include "components.hpp"
#include "systems.hpp"

void StateSystem::update(GameContext::GameContext& ctx) {
    ctx.registry
        .system<
            components::StateRegistryComponent, components::StateComponent, components::MovementComponent,
            components::AttackComponent, components::AnimationComponent>()
        .each([&ctx](
                  flecs::entity curr_entity, components::StateRegistryComponent& state_registry,
                  components::StateComponent& state, components::MovementComponent& movement,
                  components::AttackComponent& attack, components::AnimationComponent& animation
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

            std::string next_state_id = state.curr_state_id;

            if (attack.attacking) {
                if ((movement.jumping || movement.falling) && !movement.on_ground) {
                    if (curr_state.can_transition_to("attack_air")) {
                        next_state_id = "attack_air";
                    }
                } else if (curr_state.can_transition_to("attack")) {
                    next_state_id = "attack";
                }
            } else {
                if ((movement.jumping || movement.falling) && !movement.on_ground) {
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
            }

            if (next_state_id != state.curr_state_id) {
                StateEngine::State next_state;
                auto next_state_result = curr_registry.get_state(next_state_id);
                if (!next_state_result) {
                    throw std::runtime_error(next_state_result.error().message);
                }
                next_state = *next_state_result;

                StateEngine::State_can_transition_to curr_transition;
                auto state_transition_result = curr_state.get_transition(next_state_id);
                if (!state_transition_result) {
                    throw std::runtime_error(state_transition_result.error().message);
                }
                curr_transition = *state_transition_result;

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
                animation.finished = false;
            }
        })
        .run();
}
