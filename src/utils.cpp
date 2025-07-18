#include "utils.hpp"
// #include "box2d/box2d.h"
#include "clay/clay.h"
// #include "components.hpp"
// #include "entt/entt.hpp"
#include "raylib.h"
#include <iostream>

// void Utils::create_physical_body(
//     entt::registry& registry, b2WorldId world_id, const entt::entity& entity, b2Vec2 pos, b2Vec2 size, b2BodyType body_type
// ) {
//     b2BodyDef body_def = b2DefaultBodyDef();
//     body_def.type = body_type;
//     body_def.position = pos;
//     body_def.fixedRotation = true;
//     b2BodyId body_id = b2CreateBody(world_id, &body_def);
//     b2Polygon body_polygon = b2MakeBox(size.x / 2, size.y / 2);
//     b2ShapeDef body_shape_def = b2DefaultShapeDef();
//     body_shape_def.density = 1.0F;
//     body_shape_def.material.friction = 0.0F;
//     b2CreatePolygonShape(body_id, &body_shape_def, &body_polygon);
//
//     registry.emplace<Components::TransformComponent>(
//         entity, pos.x - (size.x / 2), pos.y - (size.y / 2)
//     );
//
//     registry.emplace<Components::PhysicsComponent>(entity, body_def, body_id, body_polygon, body_shape_def);
// }

auto Utils::RaylibColorToClayColor(Color rayColor) -> Clay_Color {
    Clay_Color clayColor;
    clayColor.r = rayColor.r;
    clayColor.g = rayColor.g;
    clayColor.b = rayColor.b;
    clayColor.a = rayColor.a;
    return clayColor;
}

void Utils::HandleClayErrors(Clay_ErrorData errorData) {
    std::cout << errorData.errorText.chars << "\n";
}
