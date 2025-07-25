#include "box2d/box2d.h"
#include "components.hpp"
#include "systems.hpp"

void PhysicsSensorSystem::update(flecs::world& registry, b2WorldId world_id) {
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
