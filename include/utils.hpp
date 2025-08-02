#pragma once

#include "components.hpp"
#include <clay/clay.h>
#include <raylib.h>
#include <unordered_map>

namespace Utils {

Clay_Color RaylibColorToClayColor(Color rayColor);
void HandleClayErrors(Clay_ErrorData errorData);
std::unordered_map<std::string, components::AnimationClip> generate_animation_clips(
    const std::unordered_map<std::string, std::tuple<Texture2D, bool, Vector2, Vector2, float>>& clip_input
);

}
