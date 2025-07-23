#pragma once

#include "clay/clay.h"
#include "scene_manager.hpp"

namespace Interface {

void main_menu_GUI(GameContext& ctx, SceneManager::SceneManager* scene_manager);
void game_GUI(GameContext& ctx, SceneManager::SceneManager* scene_manager);
void game_debug_GUI(flecs::world& registry);

} // namespace Interface
