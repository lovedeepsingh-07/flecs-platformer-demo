#pragma once

// #include "box2d/id.h"
// #include "box2d/math_functions.h"
// #include "box2d/types.h"
#include "clay/clay.h"
// #include "entt/entt.hpp"
#include "raylib.h"

namespace Utils {
// void create_physical_body(
//     entt::registry& registry, b2WorldId world_id, const entt::entity& entity, b2Vec2 pos, b2Vec2 size, b2BodyType body_type
// );
auto RaylibColorToClayColor(Color rayColor) -> Clay_Color;
void HandleClayErrors(Clay_ErrorData errorData);
} // namespace Utils
