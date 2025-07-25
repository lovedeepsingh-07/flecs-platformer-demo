#pragma once

#include "clay/clay.h"
#include "context.hpp"

namespace Interface {

void main_menu_GUI(GameContext& ctx);
void game_GUI(GameContext& ctx);
void game_debug_GUI(flecs::world& registry);

} // namespace Interface
