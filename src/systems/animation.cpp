#include "components.hpp"
#include "systems.hpp"

void AnimationSystem::update(GameContext& ctx) {
    flecs::system animation_sys =
        ctx.registry
            .system<components::TextureComponent, components::MovementComponent, components::AnimationComponent, components::AnimationStatesComponent>()
            .each([](components::TextureComponent& texture,
                     const components::MovementComponent& movement,
                     components::AnimationComponent& animation,
                     components::AnimationStatesComponent& animation_states) {});
    animation_sys.run();
}

// struct AnimationFrame {
//     float duration; // Duration of this frame in seconds
//     Rectangle source_rect; // Portion of the texture to draw
// };
//
// struct AnimationClip {
//     std::vector<AnimationFrame> frames;
//     bool loop;
// };
//
// struct AnimationComponent {
//     std::string current_state;
//     int current_frame_index = 0;
//     float time_accumulator = 0.0f;
//     bool playing = true;
//     bool finished = false; // useful for one-shot animations
// };
//
// struct AnimationStatesComponent {
//     std::unordered_map<std::string, AnimationClip> clips;
//     Texture2D texture;
// };

// void AnimationSystem::update(GameContext& ctx) {
//     flecs::system<components::TextureComponent, components::AnimationComponent, components::AnimationStatesComponent>()
//         .each([](components::TextureComponent& texture,
//                  components::AnimationComponent& anim,
//                  components::AnimationStatesComponent& state) {
//
//             const auto& clip = state.clips[anim.current_state];
//             if (clip.frames.empty()) return;
//
//             if (!anim.playing || anim.finished) {
//                 // Don't update animation if it's not playing or has finished
//                 return;
//             }
//
//             anim.time_accumulator += GetFrameTime();
//             const AnimationFrame& frame = clip.frames[anim.current_frame_index];
//
//             if (anim.time_accumulator >= frame.duration) {
//                 anim.time_accumulator -= frame.duration;
//                 anim.current_frame_index++;
//
//                 if (anim.current_frame_index >= clip.frames.size()) {
//                     if (clip.loop) {
//                         anim.current_frame_index = 0;
//                     } else {
//                         anim.current_frame_index = (int)clip.frames.size() - 1;
//                         anim.finished = true;
//                         anim.playing = false;
//                     }
//                 }
//             }
//
//             // Update texture rect
//             texture.texture = state.texture;
//             texture.source = clip.frames[anim.current_frame_index].source_rect;
//         });
// }

// void AnimationStateSystem::update(GameContext& ctx) {
//     flecs::system<components::MovementComponent, components::AnimationComponent, components::AnimationStatesComponent>()
//         .each([](const components::MovementComponent& movement,
//                  components::AnimationComponent& anim,
//                  const components::AnimationStatesComponent& state) {
//
//         std::string new_state;
//
//         if (!movement.on_ground) {
//             new_state = movement.jumping ? "jump" : "fall";
//         } else if (movement.left || movement.right) {
//             new_state = "run";
//         } else {
//             new_state = "idle";
//         }
//
//         if (anim.current_state != new_state) {
//             anim.current_state = new_state;
//             anim.current_frame_index = 0;
//             anim.time_accumulator = 0.0f;
//             anim.playing = true;
//             anim.finished = false;
//         }
//     });
// }

// NOTE: To make this easier, consider using tools to generate sprite frame data (like Aseprite JSON export or a custom importer) instead of manually defining rectangles.
