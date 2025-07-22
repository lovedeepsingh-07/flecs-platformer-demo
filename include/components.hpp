#pragma once

#include "box2d/id.h"
#include "raylib.h"
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
    float x;
    float y;
    float width;
    float height;
};

struct PhysicsComponent {
    b2BodyId body_id;
};
struct PhysicsSensorData {
    std::string id;
};
struct GroundSensorComponent {
    bool on_ground;
};

struct ControllerComponent {};
struct CameraComponent {};

} // namespace components
