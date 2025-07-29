#include "components.hpp"
#include "systems.hpp"

void AnimationSystem::update(GameContext& ctx) {
    flecs::system animation_sys =
        ctx.registry
            .system<components::TextureComponent, components::AnimationComponent, components::AnimationStatesComponent>()
            .each([](components::TextureComponent& texture,
                     components::AnimationComponent& animation,
                     components::AnimationStatesComponent& animation_states) {
                const components::AnimationClip& clip =
                    animation_states.clips[animation.curr_state];
                if (clip.frames.empty()) {
                    return;
                };

                if (!animation.playing || animation.finished) {
                    return;
                }

                animation.time_accumulator += GetFrameTime();
                const components::AnimationFrame& frame =
                    clip.frames[animation.curr_frame_index];

                if (animation.time_accumulator >= frame.duration) {
                    animation.time_accumulator -= frame.duration;
                    animation.curr_frame_index++;

                    if (animation.curr_frame_index >= clip.frames.size()) {
                        if (clip.loop) {
                            animation.curr_frame_index = 0;
                        } else {
                            animation.curr_frame_index = (int)clip.frames.size() - 1;
                            animation.finished = true;
                            animation.playing = false;
                        }
                    }
                }

                // Update texture rect
                texture.texture = clip.texture;
                texture.source_rect = clip.frames[animation.curr_frame_index].source_rect;
            });
    animation_sys.run();
}


// TODO: To make this easier, consider using tools to generate sprite frame data (like Aseprite JSON export or a custom importer) instead of manually defining rectangles.
