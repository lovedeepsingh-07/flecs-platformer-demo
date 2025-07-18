#pragma once

#include "box2d/types.h"
#include "flecs.h"

namespace PhysicsSystem {
void update(flecs::world& registry);
void draw_solid_polygon(
    b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* context
);
} // namespace PhysicsSystem
