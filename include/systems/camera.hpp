#pragma once

#include "flecs.h"
#include "raylib.h"

namespace CameraSystem {
void update(flecs::world& registry, Camera2D& camera);
} // namespace CameraSystem
