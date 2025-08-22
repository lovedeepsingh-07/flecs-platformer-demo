#pragma once

#include <flecs.h>

namespace observers {

void setup(flecs::world& registry);

void scene_manager(flecs::world& registry);
void movement(flecs::world& registry);
void attack(flecs::world& registry);

}
