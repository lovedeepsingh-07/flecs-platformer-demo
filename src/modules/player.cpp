#include "components.hpp"
#include "constants.hpp"
#include "modules.hpp"

void Player::setup(b2Vec2 pos, b2WorldId world_id, flecs::world& registry) {
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
    // b2Polygon body_feet_polygon =
    //     b2MakeOffsetBox(4, 3, (b2Vec2){ 0, constants::PLAYER_HEIGHT / 2 }, b2MakeRot(0));
    // b2ShapeDef body_feet_def = b2DefaultShapeDef();
    // body_feet_def.isSensor = true;
    // body_feet_def.enableSensorEvents = true;
    // b2CreatePolygonShape(body_id, &body_feet_def, &body_feet_polygon);
    flecs::entity player_entity{ registry.entity()
                                     .set(components::PositionComponent{
                                         pos.x - (size.x / 2), pos.y - (size.y / 2) })
                                     .set(components::SizeComponent{ size.x, size.y })
                                     .set(components::PhysicsComponent{ body_id })
                                     .add<components::ControllerComponent>()
                                     .add<components::CameraComponent>()
                                     .set(components::RectangleComponent{ RAYWHITE }) };
}
