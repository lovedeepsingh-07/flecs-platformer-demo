#include "components.hpp"
#include "constants.hpp"
#include "modules.hpp"
#include "utils.hpp"

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

    // texture setup
    ctx.texture_engine.load_texture("player_idle", "assets/player/idle.png");
    ctx.texture_engine.load_texture("player_run", "assets/player/run.png");
    ctx.texture_engine.load_texture("player_jump", "assets/player/jump.png");
    ctx.texture_engine.load_texture("player_land", "assets/player/land.png");
    ctx.texture_engine.load_texture("player_dash", "assets/player/dash.png");
    ctx.texture_engine.load_texture("player_attack_1", "assets/player/attack_1.png");
    ctx.texture_engine.load_texture("player_attack_2", "assets/player/attack_2.png");
    ctx.texture_engine.load_texture("player_attack_3", "assets/player/attack_3.png");
    ctx.texture_engine.load_texture("player_attack_air", "assets/player/air_attack.png");
    ctx.texture_engine.load_texture("player_hit", "assets/player/hit.png");
    ctx.texture_engine.load_texture("player_death", "assets/player/death.png");

    // animation setup
    std::unordered_map<std::string, components::AnimationClip> animation_clips = Utils::generate_animation_clips({
        { "idle",
          { ctx.texture_engine.get_texture("player_idle"), true, (Vector2){ 512, 512 }, { 256, 256 }, 0.1F } },
        { "run",
          { ctx.texture_engine.get_texture("player_run"), true, (Vector2){ 512, 512 }, { 256, 256 }, 0.1F } },
        { "jump",
          { ctx.texture_engine.get_texture("player_jump"), false, (Vector2){ 512, 512 }, { 256, 256 }, 0.1F } },
        { "land",
          { ctx.texture_engine.get_texture("player_land"), false, (Vector2){ 512, 512 }, { 256, 256 }, 0.1F } },
        { "dash",
          { ctx.texture_engine.get_texture("player_dash"), false, (Vector2){ 512, 512 }, { 256, 256 }, 0.1F } },
        { "attack_1",
          { ctx.texture_engine.get_texture("player_attack_1"), false, (Vector2){ 512, 512 }, { 256, 256 }, 0.1F } },
        { "attack_2",
          { ctx.texture_engine.get_texture("player_attack_2"), false, (Vector2){ 512, 512 }, { 256, 256 }, 0.1F } },
        { "attack_3",
          { ctx.texture_engine.get_texture("player_attack_3"), false, (Vector2){ 512, 512 }, { 256, 256 }, 0.1F } },
        { "attack_air",
          { ctx.texture_engine.get_texture("player_attack_air"), false, (Vector2){ 512, 512 }, { 256, 256 }, 0.1F } },
        { "hit",
          { ctx.texture_engine.get_texture("player_hit"), false, (Vector2){ 512, 512 }, { 256, 256 }, 0.1F } },
        { "death",
          { ctx.texture_engine.get_texture("player_death"), false, (Vector2){ 512, 512 }, { 256, 256 }, 0.1F } },
    });

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
                .texture = animation_clips["idle"].texture,
                .source_rect = animation_clips["idle"].frames[0].source_rect,
                .flipped = false,
            })
            .set(components::AnimationComponent{
                .curr_state = "idle",
            })
            .set(components::AnimationStatesComponent{
                .clips = animation_clips,
            })
    };
}
