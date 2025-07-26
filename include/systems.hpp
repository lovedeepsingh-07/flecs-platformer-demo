#pragma once

#include "box2d/types.h"
#include "context.hpp"
#include "raylib.h"

namespace CameraSystem {
void update(GameContext& ctx, Camera2D& camera);
}

namespace ControllerSystem {
void update(GameContext& ctx);
}

namespace MovementSystem {
void update(GameContext& ctx);
}

namespace PhysicsSystem {
void update(GameContext& ctx);
void draw_solid_polygon(
    b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* context
);
void draw_segment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context);
}

namespace PhysicsSensorSystem {
void update(GameContext& ctx, b2WorldId world_id);
}

namespace Render2DSystem {
void update(GameContext& ctx);
}
namespace AnimationSystem {
void update(GameContext& ctx);
}
