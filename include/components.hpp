#pragma once

#include <box2d/box2d.h>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

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
    Rectangle source_rect;
    bool flipped;
};
struct AnimationFrame {
    float duration;
    Rectangle source_rect;
};
struct AnimationClip {
    std::vector<AnimationFrame> frames;
    Texture2D texture;
    bool loop;
};
struct AnimationStatesComponent {
    std::unordered_map<std::string, AnimationClip> clips;
};
struct AnimationComponent {
    std::string curr_state;
    int curr_frame_index = 0;
    float time_accumulator = 0.0F;
    bool playing = true;
    bool finished = false;
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
};
struct AttackComponent {
    bool attacking;
};
struct JumpEventComponent {};
struct AttackEventComponent {};

struct CameraComponent {};

}
