#pragma once

#include "context.hpp"
#include <box2d/box2d.h>
#include <flecs.h>

namespace TileWorldModule {
void setup(GameContext::GameContext& ctx, b2WorldId world_id);
}

namespace PlayerModule {
void setup(b2Vec2 pos, b2WorldId world_id, GameContext::GameContext& ctx);
}
