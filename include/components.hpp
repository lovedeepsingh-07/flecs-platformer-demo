#pragma once

#include "utils.hpp"
#include <box2d/box2d.h>
#include <raylib.h>
#include <string>
#include <vector>

namespace components {

struct PositionComponent {
    float x;
    float y;
};

struct BaseColliderComponent {
    float width;
    float height;
};

struct RectangleComponent {
    float width;
    float height;
    Color color;
};

struct TextureComponent {
    Texture2D texture;
    Rectangle source_rect;
    bool flipped;
};

struct AnimationComponent {
    int curr_frame_index = 0;
    float time_accumulator = 0.0F;
    bool playing = true;
    bool finished = false;
};

struct StateRegistryComponent {
    std::string state_registry_id;
};

struct StateComponent {
    std::string curr_state_id;
};

struct PhysicsComponent {
    b2BodyId body_id;
};

struct ControllerComponent {};

struct MovementComponent {
    int8_t left_idle_right{ 0 };
    bool on_ground;
    bool jumping;
    bool falling;
};

struct AttackComponent {
    bool attacking;
};

struct PermanentRayCastComponent {
    Utils::RayCastUserData user_data;
    b2QueryFilter filter;
    Vector2 start_offset;
    Vector2 translation;
};
struct PermanentRayCastListComponent {
    std::vector<PermanentRayCastComponent> items;
};

struct JumpEventComponent {};
struct AttackEventComponent {};

struct CameraComponent {};

}
