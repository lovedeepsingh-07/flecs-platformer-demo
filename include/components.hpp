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

struct GlobalCamera {
    Camera2D camera;
};
struct Camera_Target {};

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
// RectOpts -> settings for rectangles
// Lines -> this is the lines version of a rectangle
// Debug -> this version of a rectangle is supposed to be rendered only during debug mode
struct RectOpts_Lines {};
struct RectOpts_Debug {};

struct TextureComponent {
    Texture2D texture;
    Rectangle source_rect;
    bool flipped;
};
struct State {
    std::string curr_state_id;
    std::string state_registry_id;
};
struct Animation {
    int curr_frame_index = 0;
    float time_accumulator = 0.0F;
    bool playing = true;
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

struct JumpEvent {};
struct AttackEvent {
    bool hit_some_entity;
};
struct HitEvent {
    int direction;
};

void setup(flecs::world& registry);

}
