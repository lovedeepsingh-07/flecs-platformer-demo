#include "utils.hpp"
#include "clay/clay.h"
#include "raylib.h"
#include <iostream>

Clay_Color Utils::RaylibColorToClayColor(Color rayColor) {
    Clay_Color clayColor;
    clayColor.r = rayColor.r;
    clayColor.g = rayColor.g;
    clayColor.b = rayColor.b;
    clayColor.a = rayColor.a;
    return clayColor;
}

void Utils::HandleClayErrors(Clay_ErrorData errorData) {
    std::cout << errorData.errorText.chars << "\n";
}

// first Vector2 - cavnas size of each frame
// second Vector2 - actual size of the sprite
std::unordered_map<std::string, components::AnimationClip> Utils::generate_animation_clips(
    const std::unordered_map<std::string, std::tuple<Texture2D, bool, Vector2, Vector2, float>>& clip_input
) {
    std::unordered_map<std::string, components::AnimationClip> clip_output;
    for (const auto& [key, value] : clip_input) {
        components::AnimationClip curr_clip;
        curr_clip.texture = std::get<0>(value);
        curr_clip.loop = std::get<1>(value);
        Vector2 canvas_size = std::get<2>(value);
        Vector2 sprite_size = std::get<3>(value);
        float frame_duration = std::get<4>(value);
        // NOTE: this assumes that the spritesheets are horizontal
        int total_frames = curr_clip.texture.width / (int)canvas_size.x;
        for (int i = 0; i < total_frames; i++) {
            curr_clip.frames.emplace_back((components::AnimationFrame
            ){ .duration = frame_duration,
               .source_rect = (Rectangle){
                   .x = ((float)i * canvas_size.x) + (sprite_size.x / 2),
                   // .y = 192,
                   .y = (3 * sprite_size.y) / 4, // TODO: this is highly specific for these player sprites,
                   // it assumes that the player is (3/4)*sprite_height
                   // from the top, which might be different for other
                   // sprites, you can add a `sprite_height_ratio` to the input tuple
                   .width = sprite_size.x,
                   .height = sprite_size.y,
               } });
        }
        clip_output[key] = curr_clip;
    }
    return clip_output;
};
