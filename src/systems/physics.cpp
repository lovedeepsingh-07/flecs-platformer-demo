#include "constants.hpp"
#include "systems.hpp"
#include "utils.hpp"

void systems::physics(flecs::world& registry) {
    registry.system("Step Physical World")
        .kind(flecs::PreUpdate)
        .run([](flecs::iter& iter) {
            flecs::world registry = iter.world();

            if (!registry.has<components::PhysicalWorld>()
                || !registry.has<components::ActiveScene, components::scenes::Game>()) {
                return;
            }
            auto physical_world = registry.get<components::PhysicalWorld>();

            b2World_Step(physical_world.world_id, constants::TIME_STEP, constants::SUB_STEP_COUNT);
        })
        .add<components::system_types::Update>();

    registry.system("Handle Sensor Events")
        .kind(flecs::PreUpdate)
        .run([](flecs::iter& iter) {
            flecs::world registry = iter.world();

            if (!registry.has<components::ActiveScene, components::scenes::Game>()) {
                return;
            }

            auto physical_world = registry.get<components::PhysicalWorld>();

            b2SensorEvents sensorEvents = b2World_GetSensorEvents(physical_world.world_id);
            for (int i = 0; i < sensorEvents.beginCount; ++i) {
                b2SensorBeginTouchEvent* beginTouch = sensorEvents.beginEvents + i;
                b2ShapeId sensor_id = beginTouch->sensorShapeId;
                if (b2Shape_IsSensor(sensor_id)) {
                    auto* sensor_data =
                        static_cast<game_utils::ShapeUserData*>(b2Shape_GetUserData(sensor_id));
                    if (sensor_data != nullptr && sensor_data->_id.ends_with("sensor_ground")) {
                        auto& movement =
                            sensor_data->_owner.get_mut<components::Movement>();
                        movement.on_ground = true;

                        flecs::entity jump_entity =
                            sensor_data->_owner.target<components::Jump_Entity>();
                        if (jump_entity.is_valid()) {
                            jump_entity.destruct();
                        }

                        // if the jump has been buffered, then we just convert the BufferedJumpEvent into JumpEvent
                        if (sensor_data->_owner.has<components::events::BufferedJumpEvent>()) {
                            flecs::entity jump_entity =
                                registry.entity()
                                    .add<components::events::JumpEvent, components::events::JumpEvent_One>()
                                    .child_of(sensor_data->_owner);
                            sensor_data->_owner.remove<components::events::BufferedJumpEvent>();
                        }
                    }
                }
            }
            for (int i = 0; i < sensorEvents.endCount; ++i) {
                b2SensorEndTouchEvent* endTouch = sensorEvents.endEvents + i;
                b2ShapeId sensor_id = endTouch->sensorShapeId;
                if (b2Shape_IsSensor(sensor_id)) {
                    auto* sensor_data =
                        static_cast<game_utils::ShapeUserData*>(b2Shape_GetUserData(sensor_id));
                    if (sensor_data != nullptr && sensor_data->_id.ends_with("sensor_ground")) {
                        auto& movement =
                            sensor_data->_owner.get_mut<components::Movement>();
                        movement.on_ground = false;
                    }
                }
            }
        })
        .add<components::system_types::Update>();

    registry
        .system<components::PhysicalBody, components::Position>(
            "Apply Physical Calculations"
        )
        .kind(flecs::PreUpdate)
        .each([](const components::PhysicalBody& body, components::Position& pos) {
            b2Vec2 body_pos = b2Body_GetPosition(body.body_id);
            pos.x = body_pos.x;
            pos.y = body_pos.y;
        })
        .add<components::system_types::Update>();
};
