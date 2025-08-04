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
            StateEngine::StateRegistry& curr_registry =
                ctx.state_engine.get_state_registry(state_registry.state_registry_id);
            StateEngine::State curr_state = curr_registry[state.curr_state_id];

            std::string next_state_id = state.curr_state_id;

            if (attack.attacking) {
                if ((movement.jumping || movement.falling) && !movement.on_ground) {
                    if (curr_state.can_transition_to.contains("attack_air")) {
                        next_state_id = "attack_air";
                    }
                } else if (curr_state.can_transition_to.contains("attack_1")) {
                    next_state_id = "attack_1";
                }
            } else {
                if ((movement.jumping || movement.falling) && !movement.on_ground) {
                    if (curr_state.can_transition_to.contains("jump")) {
                        next_state_id = "jump";
                    }
                } else if (movement.left_idle_right == 0) {
                    if (curr_state.can_transition_to.contains("idle")) {
                        next_state_id = "idle";
                    }
                } else {
                    if (curr_state.can_transition_to.contains("run")) {
                        next_state_id = "run";
                    }
                }
            }

            if (next_state_id != state.curr_state_id) {
                StateEngine::State next_state = curr_registry[next_state_id];
                int next_state_frame =
                    curr_state.can_transition_to[next_state_id].frame;
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
