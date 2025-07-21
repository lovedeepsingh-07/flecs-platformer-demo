#pragma once

#include "raylib.h"

namespace constants {

constexpr int FPS = 75;
constexpr int BASE_WINDOW_WIDTH = 1200;
constexpr int BASE_WINDOW_HEIGHT = 600;

constexpr float WORLD_SCALE = 16.0F;
constexpr float PLAYER_WIDTH = 16.0F;
constexpr float PLAYER_HEIGHT = 32.0F;

constexpr float FORCE_CONST = 5.0F;
constexpr float TIME_STEP = 1 / 60.0F;
constexpr int SUB_STEP_COUNT = 4;

constexpr float MIN_CAMERA_SPEED = 30;
constexpr float MIN_CAMERA_EFFECT_LENGTH = 30;
constexpr float CAMERA_FRACTION_SPEED = 0.5F;

constexpr KeyboardKey DEBUG_KEY = KEY_F3;

} // namespace constants
