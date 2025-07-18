#pragma once

#include "box2d/id.h"
#include "raylib.h"

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
// struct RectangleLinesComponent {
//     float width;
//     float height;
//     Color color;
// };
// struct TextComponent {
//     std::string text;
//     int font_size;
//     Color color;
// };
struct PhysicsComponent {
    b2BodyId body_id;
};
struct ControllerComponent {};
struct CameraComponent {};
} // namespace components
