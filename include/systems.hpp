#pragma once

#include <clay_raylib.hpp>
#include <flecs.h>

namespace systems {

void setup(flecs::world& registry);

void controller(flecs::world& registry);
void movement(flecs::world& registry);
void physics(flecs::world& registry);
void camera(flecs::world& registry);
void render(flecs::world& registry);

}
