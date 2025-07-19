#pragma once

#include "raylib.h"

namespace constants {

const int FPS = 75;
const int BASE_WINDOW_WIDTH = 1200;
const int BASE_WINDOW_HEIGHT = 600;

const float WORLD_SCALE = 16.0F;
const float PLAYER_WIDTH = 16.0F;
const float PLAYER_HEIGHT = 32.0F;

const float FORCE_CONST = 5.0F;
const float TIME_STEP = 1 / 60.0F;
const int SUB_STEP_COUNT = 4;

const float MIN_CAMERA_SPEED = 30;
const float MIN_CAMERA_EFFECT_LENGTH = 30;
const float CAMERA_FRACTION_SPEED = 0.5F;

const KeyboardKey DEBUG_KEY = KEY_F3;

} // namespace constants
