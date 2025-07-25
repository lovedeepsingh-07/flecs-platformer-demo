#include "box2d/box2d.h"
#include "components.hpp"
#include "systems.hpp"

void PhysicsSystem::update(GameContext& ctx) {
    flecs::system physics_sys =
        ctx.registry
            .system<components::PositionComponent, components::SizeComponent, components::PhysicsComponent>()
            .each([](components::PositionComponent& pos, const components::SizeComponent& size,
                     const components::PhysicsComponent& phy) {
                b2Vec2 body_pos = b2Body_GetPosition(phy.body_id);
                pos.x = body_pos.x - (size.width / 2);
                pos.y = body_pos.y - (size.height / 2);
            });
    physics_sys.run();
}

void PhysicsSystem::draw_solid_polygon(
    b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* context
) {
    for (int i = 0; i < vertexCount; i++) {
        b2Vec2 curr_vertex = b2TransformPoint(transform, vertices[i]);
        b2Vec2 next_vertex = b2TransformPoint(transform, vertices[(i + 1) % vertexCount]);

        Vector2 p0 = { curr_vertex.x, curr_vertex.y };
        Vector2 p1 = { next_vertex.x, next_vertex.y };

        DrawLineV(p0, p1, GREEN);
    }
}
void PhysicsSystem::draw_segment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context) {
    DrawLineV((Vector2){ p1.x, p1.y }, (Vector2){ p2.x, p2.y }, GREEN);
};
