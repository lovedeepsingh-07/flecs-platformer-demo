#include "components.hpp"
#include "constants.hpp"
#include "modules.hpp"
// #include "utils.hpp"

void PlayerModule::setup(b2Vec2 pos, b2WorldId world_id, GameContext::GameContext& ctx) {
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

    // texture setup
    ctx.texture_engine.load_texture("player_idle", "assets/player/idle.png");
    ctx.texture_engine.load_texture("player_walk", "assets/player/walk.png");
    ctx.texture_engine.load_texture("player_jump", "assets/player/jump.png");
    ctx.texture_engine.load_texture("player_attack", "assets/player/sword_slash_vertical.png");
    ctx.texture_engine.load_texture("player_attack_air", "assets/player/horizontal_air_slash.png");

    // state setup
    ctx.state_engine.load_state_registry("player", "data/player.states.yaml");

    StateEngine::State starting_state =
        ctx.state_engine.get_state_registry("player")["idle"];

    // ecs entity setup
    flecs::entity player_entity{
        ctx.registry.entity()
            .set(components::PositionComponent{ pos.x - (shape_size.x / 2),
                                                pos.y - (shape_size.y / 2) })
            .set(components::PhysicsComponent{ body_id })
            .set(components::BaseColliderComponent{ shape_size.x, shape_size.y })
            .set(components::MovementComponent{
                .left_idle_right = 0,
                .on_ground = false,
            })
            .add<components::ControllerComponent>()
            .add<components::CameraComponent>()
            .set(components::StateRegistryComponent{ .state_registry_id =
                                                         "player" })
            .set(components::StateComponent{
                .curr_state_id = "idle",
            })
            .set(components::TextureComponent{
                .texture =
                    ctx.texture_engine.get_texture(starting_state.animation_data.texture_id),
                .source_rect = starting_state.animation_data.frames[0],
                .flipped = false,
            })
            .set(components::AnimationComponent{})
            .set(components::AttackComponent{ .attacking = false })
    };
}
