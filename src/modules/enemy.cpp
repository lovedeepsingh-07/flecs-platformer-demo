#include "components.hpp"
#include "constants.hpp"
#include "modules.hpp"
#include "utils.hpp"
#include <vector>

void EnemyModule::setup(b2Vec2 pos, b2WorldId world_id, GameContext::GameContext& ctx) {
    // physical body setup
    b2Vec2 shape_size{ (b2Vec2){ constants::ENEMY_COLLIDER_WIDTH,
                                 constants::ENEMY_COLLIDER_HEIGHT } };
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
    body_shape_def.filter.categoryBits = Utils::EntityCategories::ENEMY;
    b2CreatePolygonShape(body_id, &body_shape_def, &body_polygon);

    // ground sensor setup
    b2Polygon ground_sensor_polygon =
        b2MakeOffsetBox(6, 4, (b2Vec2){ 0, constants::ENEMY_HEIGHT / 2 }, b2MakeRot(0));
    b2ShapeDef ground_sensor_def = b2DefaultShapeDef();
    ground_sensor_def.isSensor = true;
    ground_sensor_def.enableSensorEvents = true;
    ground_sensor_def.filter.categoryBits = Utils::EntityCategories::ENEMY;
    auto* sensor_data = new Utils::ShapeUserData{ ._id = "ground_sensor" };
    ground_sensor_def.userData = sensor_data;
    b2CreatePolygonShape(body_id, &ground_sensor_def, &ground_sensor_polygon);

    // attack raycast setup
    b2QueryFilter raycast_filter = b2DefaultQueryFilter();
    raycast_filter.maskBits = Utils::EntityCategories::PLAYER;
    Utils::RayCastUserData raycast_user_data =
        Utils::RayCastUserData{ ._id = "enemy_raycast" };
    std::vector<components::PermanentRayCastComponent> raycast_list(2);
    raycast_list.emplace_back(components::PermanentRayCastComponent{
        .user_data = raycast_user_data,
        .filter = raycast_filter,
        .start_offset = (Vector2){ 0, -constants::ENEMY_COLLIDER_HEIGHT / 4 },
        .translation = (Vector2){ constants::ENEMY_ATTACK_RAYCAST_LENGTH, 0 },
    });
    raycast_list.emplace_back(components::PermanentRayCastComponent{
        .user_data = raycast_user_data,
        .filter = raycast_filter,
        .start_offset = (Vector2){ 0, constants::ENEMY_COLLIDER_HEIGHT / 4 },
        .translation = (Vector2){ constants::ENEMY_ATTACK_RAYCAST_LENGTH, 0 },
    });

    // texture setup
    ctx.texture_engine.load_texture("enemy_idle", "assets/enemy/idle.png");
    ctx.texture_engine.load_texture("enemy_walk", "assets/enemy/walk.png");
    ctx.texture_engine.load_texture("enemy_jump", "assets/enemy/jump.png");
    ctx.texture_engine.load_texture("enemy_attack", "assets/enemy/sword_slash_vertical.png");
    ctx.texture_engine.load_texture("enemy_attack_air", "assets/enemy/horizontal_air_slash.png");

    // state setup
    ctx.state_engine.load_state_registry("enemy", "data/enemy.states.yaml");

    StateEngine::State starting_state =
        ctx.state_engine.get_state_registry("enemy")["idle"];

    // ecs entity setup
    flecs::entity enemy_entity{
        ctx.registry.entity()
            .set(components::PositionComponent{ pos.x - (shape_size.x / 2),
                                                pos.y - (shape_size.y / 2) })
            .set(components::PhysicsComponent{ body_id })
            .set(components::BaseColliderComponent{ shape_size.x, shape_size.y })
            .set(components::MovementComponent{
                .left_idle_right = 0,
                .on_ground = false,
            })
            .set(components::StateRegistryComponent{ .state_registry_id =
                                                         "enemy" })
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
            .set(components::PermanentRayCastListComponent{ .items = raycast_list })
    };
}
