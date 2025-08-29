#pragma once

#include <clay_raylib.hpp>
#include <flecs.h>

namespace systems {

void setup(flecs::world& registry);

void controller(flecs::world& registry);
void movement(flecs::world& registry);
void physics(flecs::world& registry);
void state(flecs::world& registry);
void attack(flecs::world& registry);
void camera(flecs::world& registry);
void animation(flecs::world& registry);
void particles(flecs::world& registry);

namespace render {

void setup(flecs::world& registry);

void texture(flecs::world& registry);
void rectangle(flecs::world& registry);
void physics(flecs::world& registry);
void GUI(flecs::world& registry);

}

}
