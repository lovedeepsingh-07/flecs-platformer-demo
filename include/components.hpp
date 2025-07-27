#pragma once

#include "box2d/id.h"
#include "raylib.h"
#include <string>
#include <unordered_map>

namespace components {

struct PositionComponent {
    float x;
    float y;
};
struct SizeComponent {
    float width;
    float height;
};
struct RectangleComponent {
    Color color;
};

struct TextureComponent {
    Texture2D texture;
    float x;
    float y;
    float width;
    float height;
    bool flipped;
};
struct AnimationComponent {
    uint8_t curr_frame;
    float frame_width;
    float frame_height;
    float elapsed_time;
    bool loop;
};
struct AnimationStatesComponent {
    std::unordered_map<std::string, Texture2D> states; // <animation_state, animation_texture>
};

struct PhysicsComponent {
    b2BodyId body_id;
};
struct PhysicsSensorData {
    std::string id;
};

struct ControllerComponent {};
struct MovementComponent {
    bool left;
    bool right;
    bool on_ground;
    bool jumping;
    bool falling;
    bool jump_requested;
};

struct CameraComponent {};

}
