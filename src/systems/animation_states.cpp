#include "components.hpp"
#include "systems.hpp"

void AnimationStatesSystem::update(GameContext& ctx) {
    flecs::system texture_flipping_sys =
        ctx.registry
            .system<components::TextureComponent, components::MovementComponent>()
            .each([](components::TextureComponent& texture,
                     const components::MovementComponent& movement) {
                if (movement.left) {
                    texture.flipped = true;
                }
                if (movement.right) {
                    texture.flipped = false;
                }
            });
    texture_flipping_sys.run();

    flecs::system animation_states_sys =
        ctx.registry
            .system<components::MovementComponent, components::AnimationComponent, components::AnimationStatesComponent>()
            .each([](const components::MovementComponent& movement,
                     components::AnimationComponent& animation,
                     components::AnimationStatesComponent& animation_states) {
                std::string new_state;

                if (!movement.on_ground) {
                    new_state = "jump";
                } else if (movement.left || movement.right) {
                    new_state = "run";
                } else {
                    new_state = "idle";
                }

                if (animation.curr_state != new_state) {
                    animation.curr_state = new_state;
                    animation.curr_frame_index = 0;
                    animation.time_accumulator = 0.0F;
                    animation.playing = true;
                    animation.finished = false;
                }
            });
    animation_states_sys.run();
}
