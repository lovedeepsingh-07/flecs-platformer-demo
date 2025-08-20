#pragma once

#include "state_engine.hpp"
#include "texture_engine.hpp"
#include <array>
#include <box2d/box2d.h>
#include <flecs.h>
#include <raylib.h>

namespace components {

struct Paused {};
struct Freezed {
    float freeze_time;
};
struct DebugMode {};
struct PhysicalDebugDraw {
    b2DebugDraw debug_draw;
};

struct SceneRoot {};
// this represents an exclusive relationship (there can be only 1 active scene)
struct ActiveScene {};
// these below are the "targets" of that relationship
struct MainMenu_Scene {};
struct Game_Scene {};

struct Texture_Engine {
    TextureEngine::TextureEngine engine;
};
struct State_Engine {
    StateEngine::StateEngine engine;
};


struct GameFonts {
    std::array<Font, 1> font_list;
};

struct Controller {
    uint8_t _id;
};

struct Position {
    float x;
    float y;
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

struct PhysicalWorld {
    b2WorldId world_id;
};
struct PhysicalBody {
    b2BodyId body_id;
};
struct BaseCollider {
    float width;
    float height;
};

struct Movement {
    int left_idle_right{ 0 };
    bool on_ground;
};

void setup(flecs::world& registry);

}
