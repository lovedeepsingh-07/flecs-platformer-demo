#pragma once

#include <array>
#include <box2d/box2d.h>
#include <flecs.h>
#include <raylib.h>

namespace components {

struct Paused {};
struct Freezed {
    float freeze_time;
};

struct SceneRoot {};
// this represents an exclusive relationship (there can be only 1 active scene)
struct ActiveScene {};
// these below are the "targets" of that relationship
struct MainMenu_Scene {};
struct Game_Scene {};

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

struct Rectangle {
    float width;
    float height;
    Color color;
};

void setup(flecs::world& registry);

}
