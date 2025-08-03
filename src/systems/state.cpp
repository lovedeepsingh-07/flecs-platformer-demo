#include "components.hpp"
#include "systems.hpp"

void StateSystem::update(GameContext::GameContext& ctx) {
    flecs::query<> state_query = ctx.registry.query_builder()
                                     .with<components::StateRegistryComponent>()
                                     .with<components::StateComponent>()
                                     .with<components::MovementComponent>()
                                     .with<components::AnimationComponent>()
                                     .build();
    ctx.registry.defer_begin();
    state_query.run([&ctx](flecs::iter& iter) {
        while (iter.next()) {
            auto state_registry = iter.field<components::StateRegistryComponent>(0);
            auto state = iter.field<components::StateComponent>(1);
            auto movement = iter.field<components::MovementComponent>(2);
            auto animation = iter.field<components::AnimationComponent>(3);

            for (auto i : iter) {
                flecs::entity curr_entity = iter.entity(i);

                StateEngine::StateRegistry& curr_registry =
                    ctx.state_engine.get_state_registry(state_registry[i].state_registry_id);
                StateEngine::State curr_state = curr_registry[state[i].curr_state_id];

                std::string next_state_id = state[i].curr_state_id;

                if ((movement[i].jumping || movement[i].falling) && !movement[i].on_ground) {
                    if (curr_state.can_transition_to.contains("jump")) {
                        next_state_id = "jump";
                    }
                } else if (movement[i].left_idle_right == 0) {
                    if (curr_state.can_transition_to.contains("idle")) {
                        next_state_id = "idle";
                    }
                } else {
                    if (curr_state.can_transition_to.contains("run")) {
                        next_state_id = "run";
                    }
                }

                if (next_state_id != state[i].curr_state_id) {
                    StateEngine::State next_state = curr_registry[next_state_id];
                    int next_state_frame =
                        curr_state.can_transition_to[next_state_id].frame;
                    if (next_state_frame == -1) {
                        animation[i].curr_frame_index =
                            (int)next_state.animation_data.frames.size() - 1;
                    } else {
                        animation[i].curr_frame_index = next_state_frame;
                    }

                    state[i].curr_state_id = next_state_id;
                    animation[i].curr_frame_index = 0;
                    animation[i].time_accumulator = 0.0F;
                    animation[i].playing = true;
                    animation[i].finished = false;
                }
            }
        }
    });
    ctx.registry.defer_end();
}
