#include "components.hpp"
#include "systems.hpp"

void AnimationSystem::update(GameContext& ctx) {
    flecs::system animation_sys =
        ctx.registry
            .system<components::TextureComponent, components::MovementComponent, components::AnimationComponent, components::AnimationStatesComponent>()
            .each([](components::TextureComponent& texture,
                     const components::MovementComponent& movement,
                     components::AnimationComponent& animation,
                     components::AnimationStatesComponent& animation_states) {
                // texture flipping
                if (movement.left) {
                    texture.flipped = true;
                }
                if (movement.right) {
                    texture.flipped = false;
                }

                // animation states
                if (movement.left || movement.right) {
                    texture.texture = animation_states.states["run"];
                } else {
                    texture.texture = animation_states.states["idle"];
                }

                // actual animation logic
                int total_frames = texture.texture.width / (int)animation.frame_width;
                texture.x = ((float)animation.curr_frame * animation.frame_width);
                if (animation.curr_frame == total_frames - 1) {
                    animation.curr_frame = 0;
                } else if (animation.elapsed_time >= 0.06F) {
                    animation.curr_frame += 1;
                    animation.elapsed_time = 0.0F;
                }
                animation.elapsed_time += GetFrameTime();
            });
    animation_sys.run();
}
