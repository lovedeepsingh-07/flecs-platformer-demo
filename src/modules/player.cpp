#include "components.hpp"
#include "constants.hpp"
#include "modules.hpp"

void Player::setup(b2Vec2 pos, b2WorldId world_id, flecs::world& registry) {
    // physical body setup
    b2Vec2 size{ (b2Vec2){ constants::PLAYER_WIDTH, constants::PLAYER_HEIGHT } };
    b2BodyDef body_def{ b2DefaultBodyDef() };
    body_def.type = b2_dynamicBody;
    body_def.position = pos;
    body_def.fixedRotation = true;
    b2BodyId body_id = b2CreateBody(world_id, &body_def);
    b2Polygon body_polygon = b2MakeBox(size.x / 2, size.y / 2);
    b2ShapeDef body_shape_def = b2DefaultShapeDef();
    body_shape_def.density = 1.0F;
    body_shape_def.material.friction = 0.0F;
    b2CreatePolygonShape(body_id, &body_shape_def, &body_polygon);

    // ground sensor setup
    b2Polygon ground_sensor_polygon =
        b2MakeOffsetBox(6, 4, (b2Vec2){ 0, constants::PLAYER_HEIGHT / 2 }, b2MakeRot(0));
    b2ShapeDef ground_sensor_def = b2DefaultShapeDef();
    ground_sensor_def.isSensor = true;
    ground_sensor_def.enableSensorEvents = true;
    auto* sensor_data =
        new components::PhysicsSensorData{ .id = "ground_sensor" };
    ground_sensor_def.userData = sensor_data;
    b2CreatePolygonShape(body_id, &ground_sensor_def, &ground_sensor_polygon);

    // ecs entity setup
    flecs::entity player_entity{
        registry.entity()
            .set(components::PositionComponent{ pos.x - (size.x / 2), pos.y - (size.y / 2) })
            .set(components::SizeComponent{ size.x, size.y })
            .set(components::PhysicsComponent{ body_id })
            .set(components::GroundSensorComponent{ .on_ground = false })
            .add<components::ControllerComponent>()
            .add<components::CameraComponent>()
            .set(components::RectangleComponent{ RAYWHITE })
    };
}
