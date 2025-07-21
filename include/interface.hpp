#pragma once

#include "clay/clay.h"
#include "screen_manager.hpp"

namespace Interface {

void main_menu_GUI(ScreenManager::ScreenManager* screen_manager);
void game_GUI(ScreenManager::ScreenManager* screen_manager);
void game_debug_GUI(flecs::world& registry);

} // namespace Interface
