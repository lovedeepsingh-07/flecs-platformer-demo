#pragma once

#include <raylib.h>

namespace constants {
    constexpr Color BACKGROUND_COLOR = BLACK;

    constexpr int TARGET_FPS = 90;
    constexpr float FRAMES_TO_SEC = 1.0F / TARGET_FPS;
    constexpr float SEC_TO_FRAMES = TARGET_FPS;

    constexpr int BASE_WINDOW_WIDTH = 1200;
    constexpr int BASE_WINDOW_HEIGHT = 600;

    constexpr float WORLD_SCALE = 32.0F;
    constexpr float FORCE_CONST = 2.0F;
    constexpr float TIME_STEP = 1 / 60.0F;
    constexpr int SUB_STEP_COUNT = 4;

    constexpr float PLAYER_WIDTH = 32.0F;
    constexpr float PLAYER_HEIGHT = 48.0F;
    constexpr float PLAYER_COLLIDER_WIDTH = 32.0F;
    constexpr float PLAYER_COLLIDER_HEIGHT = 48.0F;
    constexpr float PLAYER_ATTACK_RAYCAST_LENGTH = 56.0F;
    constexpr float MAX_PLAYER_HEALTH = 100.0F;

    constexpr float PLAYER_JUMP_VEL = -5.0F * WORLD_SCALE * FORCE_CONST;
    constexpr float PLAYER_DASH_VEL = 5.0F * WORLD_SCALE * FORCE_CONST;
    constexpr float MAX_PLAYER_RUN_VEL = 3.0F * WORLD_SCALE * FORCE_CONST;
    constexpr float ACCEL_GROUND = 12.0F * WORLD_SCALE * FORCE_CONST;
    constexpr float ACCEL_AIR = 5.0F * WORLD_SCALE * FORCE_CONST;
    constexpr float DECEL_GROUND = 12.0F * WORLD_SCALE * FORCE_CONST;
    constexpr float DECEL_AIR = 2.0F * WORLD_SCALE * FORCE_CONST;

    constexpr float ENEMY_WIDTH = 32.0F;
    constexpr float ENEMY_HEIGHT = 48.0F;
    constexpr float ENEMY_COLLIDER_WIDTH = 32.0F;
    constexpr float ENEMY_COLLIDER_HEIGHT = 48.0F;
    constexpr float ENEMY_ATTACK_RAYCAST_LENGTH = 56.0F;
    constexpr float ENEMY_JUMP_VEL = -5.0F * WORLD_SCALE * FORCE_CONST;
    constexpr float MAX_ENEMY_HEALTH = 100.0F;

    constexpr float MIN_CAMERA_SPEED = 30;
    constexpr float MIN_CAMERA_EFFECT_LENGTH = 30;
    constexpr float CAMERA_FRACTION_SPEED = 0.5F;

    constexpr float HEALTHBAR_HEIGHT = 8.0F;
    constexpr float HEALTHBAR_WIDTH = 64.0F;

    constexpr float ANIMATION_FRAME_TIME = 0.1F;

    constexpr KeyboardKey DEBUG_KEY = KEY_F2;
}
