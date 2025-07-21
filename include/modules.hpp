#pragma once

#include "box2d/box2d.h"
#include "flecs.h"

namespace TileWorld {
void setup(flecs::world& registry, b2WorldId world_id);
}

namespace Player {
void setup(b2Vec2 pos, b2WorldId world_id, flecs::world& registry);
}
