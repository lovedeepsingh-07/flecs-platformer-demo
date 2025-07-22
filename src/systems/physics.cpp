#include "box2d/box2d.h"
#include "components.hpp"
#include "systems.hpp"

void PhysicsSystem::update(flecs::world& registry, b2WorldId world_id) {
    flecs::system physics_sys =
        registry
            .system<components::PositionComponent, components::SizeComponent, components::PhysicsComponent>()
            .each([world_id](
                      components::PositionComponent& pos, const components::SizeComponent& size,
                      const components::PhysicsComponent& phy
                  ) {
                b2Vec2 body_pos = b2Body_GetPosition(phy.body_id);
                pos.x = body_pos.x - (size.width / 2);
                pos.y = body_pos.y - (size.height / 2);
            });
    physics_sys.run();

    flecs::query<> sensor_query =
        registry.query_builder().with<components::GroundSensorComponent>().build();

    sensor_query.run([&](flecs::iter& it) {
        while (it.next()) {
            b2SensorEvents sensorEvents = b2World_GetSensorEvents(world_id);
            for (int i = 0; i < sensorEvents.beginCount; ++i) {
                b2SensorBeginTouchEvent* beginTouch = sensorEvents.beginEvents + i;
                b2ShapeId shape_id = beginTouch->sensorShapeId;
                if (b2Shape_IsSensor(shape_id)) {
                    auto* sensor_data =
                        static_cast<components::PhysicsSensorData*>(b2Shape_GetUserData(shape_id));
                    if (sensor_data != nullptr && sensor_data->id == "ground_sensor") {
                        auto ground_sensor =
                            it.field<components::GroundSensorComponent>(0);
                        for (auto i : it) {
                            auto curr_entity = it.entity(i);
                            if (curr_entity.has<components::GroundSensorComponent>()) {
                                ground_sensor[i].on_ground = true;
                            }
                        }
                    }
                }
            }
            for (int i = 0; i < sensorEvents.endCount; ++i) {
                b2SensorEndTouchEvent* endTouch = sensorEvents.endEvents + i;
                b2ShapeId shape_id = endTouch->sensorShapeId;
                if (b2Shape_IsSensor(shape_id)) {
                    auto* sensor_data =
                        static_cast<components::PhysicsSensorData*>(b2Shape_GetUserData(shape_id));
                    if (sensor_data != nullptr && sensor_data->id == "ground_sensor") {
                        auto ground_sensor =
                            it.field<components::GroundSensorComponent>(0);
                        for (auto i : it) {
                            auto curr_entity = it.entity(i);
                            if (curr_entity.has<components::GroundSensorComponent>()) {
                                ground_sensor[i].on_ground = false;
                            }
                        }
                    }
                }
            }
        }
    });
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
