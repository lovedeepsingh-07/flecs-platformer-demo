#pragma once

#include "box2d/types.h"
#include "flecs.h"
#include "raylib.h"

namespace CameraSystem {
void update(flecs::world& registry, Camera2D& camera);
} // namespace CameraSystem

namespace ControllerSystem {
void update(flecs::world& registry);
} // namespace ControllerSystem

namespace PhysicsSystem {

void update(flecs::world& registry);
void draw_solid_polygon(
    b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* context
);
void draw_segment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context);

} // namespace PhysicsSystem

namespace PhysicsSensorSystem {
void update(flecs::world& registry, b2WorldId world_id);
} // namespace PhysicsSensorSystem

namespace Render2DSystem {
void update(flecs::world& registry);
} // namespace Render2DSystem
