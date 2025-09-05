#include "components.hpp"
#include "constants.hpp"
#include "scene.hpp"
#include "utils.hpp"

void scene::game::setup_player(flecs::world& registry, b2WorldId world_id, b2Vec2 pos) {
    flecs::entity scene_root = registry.lookup("scene_root");

    auto& texture_engine = registry.get_mut<components::Texture_Engine>();

    flecs::entity player_entity = registry.entity("player")
                                      .set_alias("player")
                                      .set<components::Controller>({ 0 })
                                      .add<components::Camera_Target>()
                                      .add<components::Animation>()
                                      .child_of(scene_root);

    // physical body setup
    b2Vec2 shape_size{ (b2Vec2){ constants::PLAYER_COLLIDER_WIDTH,
                                 constants::PLAYER_COLLIDER_HEIGHT } };
    b2BodyDef body_def{ b2DefaultBodyDef() };
    body_def.type = b2_dynamicBody;
    body_def.position = pos;
    body_def.fixedRotation = true;
    body_def.gravityScale = 1.0F;
    b2BodyId body_id = b2CreateBody(world_id, &body_def);
    b2Polygon body_polygon = b2MakeBox(shape_size.x / 2, shape_size.y / 2);
    b2ShapeDef body_shape_def = b2DefaultShapeDef();
    body_shape_def.density = 1.0F;
    body_shape_def.material.friction = 0.0F;
    body_shape_def.material.restitution = 0.0F;
    body_shape_def.filter.categoryBits = game_utils::EntityCategories::PLAYER;
    body_shape_def.filter.maskBits = game_utils::EntityCategories::GROUND;
    body_shape_def.userData =
        new game_utils::ShapeUserData{ ._id = "player_shape_body", ._owner = player_entity };
    b2CreatePolygonShape(body_id, &body_shape_def, &body_polygon);

    // ground sensor setup
    b2Polygon ground_sensor_polygon = b2MakeOffsetBox(
        constants::PLAYER_WIDTH / 2, 4,
        (b2Vec2){ 0, constants::PLAYER_HEIGHT / 2 }, b2MakeRot(0)
    );
    b2ShapeDef ground_sensor_def = b2DefaultShapeDef();
    ground_sensor_def.isSensor = true;
    ground_sensor_def.enableSensorEvents = true;
    ground_sensor_def.filter.categoryBits = game_utils::EntityCategories::PLAYER;
    ground_sensor_def.filter.maskBits = game_utils::EntityCategories::GROUND;
    ground_sensor_def.userData =
        new game_utils::ShapeUserData{ ._id = "player_sensor_ground", ._owner = player_entity };
    b2CreatePolygonShape(body_id, &ground_sensor_def, &ground_sensor_polygon);

    // setup cast query filter
    b2QueryFilter cast_query_filter = b2DefaultQueryFilter();
    cast_query_filter.maskBits = game_utils::EntityCategories::ENEMY;

    player_entity.set(components::Position{ pos.x, pos.y })
        .set(components::PhysicalBody{ body_id })
        .set(components::CastQueryFilter{ cast_query_filter })
        .set(components::BaseCollider{ shape_size.x, shape_size.y })
        .set(components::Movement{ .left_idle_right = 0, .on_ground = false });

    // state setup
    auto& state_engine = registry.get_mut<components::State_Engine>();
    auto state_registry_result = state_engine.engine.get_state_registry("cowboy");
    if (!state_registry_result) {
        throw std::runtime_error(state_registry_result.error().message);
    }
    StateEngine::StateRegistry curr_registry = *state_registry_result;
    auto state_result = curr_registry.get_state("idle");
    if (!state_result) {
        throw std::runtime_error(state_result.error().message);
    }
    StateEngine::State starting_state = *state_result;
    player_entity.set<components::State>({ "idle", "cowboy" });

    TextureEngine::GameTexture player_texture =
        texture_engine.engine.get_texture(starting_state.animation_data.texture_id);
    player_entity.set<components::TextureComponent>({
        .texture = player_texture.texture,
        .source_rect = starting_state.animation_data.frames[0].source_rect,
        .render_scale = player_texture.render_scale,
        .flipped = false,
    });

    // setup health
    player_entity.set(components::Health{
        .health = constants::MAX_PLAYER_HEALTH,
        .max_health = constants::MAX_PLAYER_HEALTH,
    });


    // setup partile emitters
    auto jump_particle_engine = ParticleEngine::ParticleEngine{};
    jump_particle_engine.pool_size = 360; // somewhat arbitrary number
    jump_particle_engine.emitting = false;
    jump_particle_engine.config = ParticleEngine::EngineConfig{
        .local_coords = false,
        .one_shot = true,
        .amount = 4,
        .speed_scale = 2.5F,
        .lifetime = 0.25F,
        .velocity_scale = 100.0F,
        .explosiveness = 0.0F,
        .direction_bias = -std::numbers::pi / 2,
        .spread = 10.0F * DEG2RAD,
        .separation = 155 * DEG2RAD,
        .square_particles = true,
        .start_size = 4.6F,
        .end_size = 3.5F,
        .start_color = Vector4{ 1.0F, 1.0F, 1.0F, 1.0F },
        .end_color = Vector4{ 1.0F, 1.0F, 1.0F, 0.0F },
    };
    jump_particle_engine.setup();
    flecs::entity jump_emitter_entity =
        registry.entity()
            .set<components::Particle_Emitter>({ jump_particle_engine })
            .set<components::Position>({ pos.x, pos.y + (constants::PLAYER_COLLIDER_HEIGHT / 2) })
            .child_of(player_entity);
    player_entity.add<components::emitter_types::Jump>(jump_emitter_entity);

    auto dash_particle_engine = ParticleEngine::ParticleEngine{};
    dash_particle_engine.pool_size = 360; // somewhat arbitrary number
    dash_particle_engine.emitting = false;
    dash_particle_engine.config = ParticleEngine::EngineConfig{
        .local_coords = false,
        .one_shot = true,
        .amount = 4,
        .speed_scale = 2.5F,
        .lifetime = 0.25F,
        .velocity_scale = 100.0F,
        .explosiveness = 0.0F,
        .direction_bias = std::numbers::pi,
        .spread = 45.0F * DEG2RAD,
        .separation = 0,
        .square_particles = true,
        .start_size = 4.6F,
        .end_size = 3.5F,
        .start_color = Vector4{ 1.0F, 1.0F, 1.0F, 1.0F },
        .end_color = Vector4{ 1.0F, 1.0F, 1.0F, 0.0F },
    };
    dash_particle_engine.setup();
    flecs::entity dash_emitter_entity =
        registry.entity()
            .set<components::Particle_Emitter>({ dash_particle_engine })
            .set<components::Position>({ pos.x, pos.y + (constants::PLAYER_COLLIDER_HEIGHT / 2) })
            .child_of(player_entity);
    player_entity.add<components::emitter_types::Dash>(dash_emitter_entity);
}
