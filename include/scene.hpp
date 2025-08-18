#pragma once

#include "components.hpp"
#include <flecs.h>

namespace scene {

void main_menu(flecs::iter& iter, std::size_t, components::ActiveScene);
void game(flecs::iter& iter, std::size_t, components::ActiveScene);

};
