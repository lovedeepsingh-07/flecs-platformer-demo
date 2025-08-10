#include "components.hpp"
#include "systems.hpp"
#include "utils.hpp"
#include <box2d/box2d.h>

void PhysicsSensorSystem::update(GameContext::GameContext& ctx, b2WorldId world_id) {
    ctx.registry.system<components::MovementComponent>()
        .each([&world_id](flecs::entity curr_entity, components::MovementComponent& movement) {
            b2SensorEvents sensorEvents = b2World_GetSensorEvents(world_id);
            for (int i = 0; i < sensorEvents.beginCount; ++i) {
                b2SensorBeginTouchEvent* beginTouch = sensorEvents.beginEvents + i;
                b2ShapeId shape_id = beginTouch->sensorShapeId;
                if (b2Shape_IsSensor(shape_id)) {
                    auto* sensor_data =
                        static_cast<Utils::ShapeUserData*>(b2Shape_GetUserData(shape_id));
                    if (sensor_data != nullptr && sensor_data->_id == "ground_sensor") {
                        movement.on_ground = true;
                    }
                }
            }
            for (int i = 0; i < sensorEvents.endCount; ++i) {
                b2SensorEndTouchEvent* endTouch = sensorEvents.endEvents + i;
                b2ShapeId shape_id = endTouch->sensorShapeId;
                if (b2Shape_IsSensor(shape_id)) {
                    auto* sensor_data =
                        static_cast<Utils::ShapeUserData*>(b2Shape_GetUserData(shape_id));
                    if (sensor_data != nullptr && sensor_data->_id == "ground_sensor") {
                        movement.on_ground = false;
                    }
                }
            }
        })
        .run();
}
