#include "constants.hpp"
#include "scene.hpp"
#include "utils.hpp"

void scene::game::setup_enemy(flecs::world& registry, b2WorldId world_id, b2Vec2 pos) {
    flecs::entity scene_root = registry.component<components::SceneRoot>();

    auto& texture_engine = registry.get_mut<components::Texture_Engine>();

    flecs::entity enemy_entity = registry.entity("enemy")
                                     .set_alias("enemy")
                                     .set<components::Controller>({ 1 })
                                     .child_of(scene_root);

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
    body_shape_def.filter.categoryBits = game_utils::EntityCategories::ENEMY;
    body_shape_def.filter.maskBits = game_utils::EntityCategories::GROUND;
    body_shape_def.userData =
        new game_utils::ShapeUserData{ ._id = "enemy_shape_body", ._owner = enemy_entity };
    b2CreatePolygonShape(body_id, &body_shape_def, &body_polygon);

    // ground sensor setup
    b2Polygon ground_sensor_polygon = b2MakeOffsetBox(
        constants::PLAYER_WIDTH / 2, 4,
        (b2Vec2){ 0, constants::PLAYER_HEIGHT / 2 }, b2MakeRot(0)
    );
    b2ShapeDef ground_sensor_def = b2DefaultShapeDef();
    ground_sensor_def.isSensor = true;
    ground_sensor_def.enableSensorEvents = true;
    ground_sensor_def.filter.categoryBits = game_utils::EntityCategories::ENEMY;
    ground_sensor_def.filter.maskBits = game_utils::EntityCategories::GROUND;
    ground_sensor_def.userData =
        new game_utils::ShapeUserData{ ._id = "enemy_sensor_ground", ._owner = enemy_entity };
    b2CreatePolygonShape(body_id, &ground_sensor_def, &ground_sensor_polygon);

    enemy_entity
        .set(components::Position{ pos.x - (shape_size.x / 2),
                                   pos.y - (shape_size.y / 2) })
        .set(components::PhysicalBody{ body_id })
        .set(components::BaseCollider{ shape_size.x, shape_size.y })
        .set(components::Movement{ .left_idle_right = 0, .on_ground = false });

    // // load textures
    // texture_engine.engine.load_texture("player_idle", "assets/player/idle.png");
    // texture_engine.engine.load_texture("player_walk", "assets/player/walk.png");
    // texture_engine.engine.load_texture("player_jump", "assets/player/jump.png");
    // texture_engine.engine.load_texture("player_attack", "assets/player/sword_slash_vertical.png");
    // texture_engine.engine.load_texture("player_attack_air", "assets/player/horizontal_air_slash.png");

    enemy_entity.set<components::TextureComponent>({
        .texture = texture_engine.engine.get_texture("player_walk"),
        .source_rect = { 0, 0, 128, 128 },
        .flipped = false,
    });
}
