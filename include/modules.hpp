#pragma once

#include "box2d/box2d.h"
#include "context.hpp"
#include "flecs.h"

namespace TileWorldModule {
void setup(GameContext& ctx, b2WorldId world_id);
}

namespace PlayerModule {
void setup(b2Vec2 pos, b2WorldId world_id, GameContext& ctx);
}
