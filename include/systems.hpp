#pragma once

#include "context.hpp"
#include <box2d/box2d.h>
#include <raylib.h>

namespace CameraSystem {
void update(GameContext::GameContext& ctx, Camera2D& camera);
}

namespace ControllerSystem {
void update(GameContext::GameContext& ctx);
}

namespace MovementSystem {
void update(GameContext::GameContext& ctx);
}
namespace AttackSystem {
void update(GameContext::GameContext& ctx);
}

namespace PhysicsSystem {
void update(GameContext::GameContext& ctx);
void draw_solid_polygon(
    b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* context
);
void draw_segment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context);
}

namespace PhysicsSensorSystem {
void update(GameContext::GameContext& ctx, b2WorldId world_id);
}

namespace Render2DSystem {
void update(GameContext::GameContext& ctx);
}
namespace StateSystem {
void update(GameContext::GameContext& ctx);
}
namespace AnimationSystem {
void update(GameContext::GameContext& ctx);
}
