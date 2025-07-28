#include "components.hpp"
#include "constants.hpp"
#include "modules.hpp"
#include <iostream>

void PlayerModule::setup(b2Vec2 pos, b2WorldId world_id, GameContext& ctx) {
    // physical body setup
    b2Vec2 shape_size{ (b2Vec2){ constants::PLAYER_COLLIDER_WIDTH,
                                 constants::PLAYER_COLLIDER_HEIGHT } };
    b2BodyDef body_def{ b2DefaultBodyDef() };
    body_def.type = b2_dynamicBody;
    body_def.position = pos;
    body_def.fixedRotation = true;
    b2BodyId body_id = b2CreateBody(world_id, &body_def);
    b2Polygon body_polygon = b2MakeBox(shape_size.x / 2, shape_size.y / 2);
    b2ShapeDef body_shape_def = b2DefaultShapeDef();
    body_shape_def.density = 1.0F;
    body_shape_def.material.friction = 0.0F;
    body_shape_def.material.restitution = 0.0F;
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

    std::unordered_map<std::string, components::AnimationStateData> animation_states = {
        { "idle", { LoadTexture("assets/player/idle.png"), false } },
        { "run", { LoadTexture("assets/player/run.png"), false } },
        { "jump", { LoadTexture("assets/player/jump.png"), false } },
        { "land", { LoadTexture("assets/player/land.png"), false } },
        { "dash", { LoadTexture("assets/player/dash.png"), false } },
    };

    // ecs entity setup
    flecs::entity player_entity{
        ctx.registry.entity()
            .set(components::PositionComponent{ pos.x - (shape_size.x / 2),
                                                pos.y - (shape_size.y / 2) })
            .set(components::SizeComponent{ 96, 90 }) // how big we want to render the texture
            .set(components::PhysicsComponent{ body_id })
            .set(components::MovementComponent{
                .left = false, .right = false, .on_ground = false, .jump_requested = false })
            .add<components::ControllerComponent>()
            .add<components::CameraComponent>()
            .set(components::TextureComponent{
                .texture = animation_states["idle"].texture, .x = 128, .y = 192, .width = 256, .height = 256, .flipped = false }
            ) // the location and size of the rectangle from the texture that we want to render
            .set(components::AnimationComponent{
                .curr_frame = 0,
                .frame_width = 512,
                .frame_height = 512,
                .elapsed_time = 0.0F,
            }) // aztual size of each frame in the texture
            .set(components::AnimationStatesComponent{ .curr_state = "idle", .states = animation_states })
    };
}
