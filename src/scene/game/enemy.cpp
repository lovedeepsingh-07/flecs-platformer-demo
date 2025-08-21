#include "constants.hpp"
#include "scene.hpp"
#include "utils.hpp"

void scene::game::setup_enemy(flecs::world& registry, b2WorldId world_id, b2Vec2 pos) {
    flecs::entity scene_root = registry.component<components::SceneRoot>();

    auto& texture_engine = registry.get_mut<components::Texture_Engine>();

    flecs::entity enemy_entity = registry.entity("enemy")
                                     .set_alias("enemy")
                                     .set<components::Controller>({ 1 })
                                     .add<components::Animation>()
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

    // state setup
    auto& state_engine = registry.get_mut<components::State_Engine>();
    auto registry_load_result =
        state_engine.engine.load_state_registry("enemy", "data/enemy.states.yaml"); // here we load the enemy state configuration file
    if (!registry_load_result) {
        throw std::runtime_error(registry_load_result.error().message);
    }
    auto state_registry_result = state_engine.engine.get_state_registry("enemy");
    if (!state_registry_result) {
        throw std::runtime_error(state_registry_result.error().message);
    }
    StateEngine::StateRegistry curr_registry = *state_registry_result;
    auto state_result = curr_registry.get_state("idle");
    if (!state_result) {
        throw std::runtime_error(state_result.error().message);
    }
    StateEngine::State starting_state = *state_result;
    enemy_entity.set<components::State>({ "walk", "enemy" });

    enemy_entity.set<components::TextureComponent>({
        .texture =
            texture_engine.engine.get_texture(starting_state.animation_data.texture_id),
        .source_rect = { 0, 0, 128, 128 },
        .flipped = false,
    });
}
