#pragma once

#include <clay/clay.h>
#include <flecs.h>
#include <raylib.h>

namespace Interface {
    void main_menu_GUI(flecs::world& registry);
    void setting_GUI(flecs::world& registry);
    void game_setup_GUI(flecs::world& registry);
    void game_GUI(flecs::world& registry);
    void game_debug_GUI(flecs::world& registry);
};
