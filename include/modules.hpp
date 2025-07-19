#pragma once

#include "LDtkLoader/Project.hpp"
#include "box2d/box2d.h"
#include "flecs.h"

namespace TileWorld {
void setup(flecs::world& registry, b2WorldId world_id, ldtk::Project& ldtk_project);
}

namespace Player {
void setup(b2Vec2 pos, b2WorldId world_id, flecs::world& registry);
}
