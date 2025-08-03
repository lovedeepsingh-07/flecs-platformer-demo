#pragma once

#include <box2d/box2d.h>
#include <raylib.h>
#include <string>

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
struct PhysicsSensorData {
    std::string id;
};

struct ControllerComponent {};
struct MovementComponent {
    int8_t left_idle_right{ 0 };
    bool on_ground;
    bool jumping;
    bool falling;
};
// struct AttackComponent {
//     bool attacking;
// };
struct JumpEventComponent {};
// struct AttackEventComponent {};

struct CameraComponent {};

}
