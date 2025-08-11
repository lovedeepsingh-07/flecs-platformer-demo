#include "components.hpp"
#include "constants.hpp"
#include "interface.hpp"
#include "modules.hpp"
#include "scene.hpp"
#include "systems.hpp"
#include <box2d/box2d.h>
#include <rlImGui.h>

void Scene::GameScene::on_enter(GameContext::GameContext& ctx) {
    rlImGuiSetup(true);
    b2SetLengthUnitsPerMeter(constants::WORLD_SCALE);
    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity.y = 9.8F * constants::WORLD_SCALE * constants::FORCE_CONST;
    m_world_id = b2CreateWorld(&world_def);
    m_world_debug_draw = b2DefaultDebugDraw();
    m_world_debug_draw.drawShapes = true;
    m_world_debug_draw.drawContacts = true;
    m_world_debug_draw.drawBounds = true;
    m_world_debug_draw.DrawSolidPolygonFcn = PhysicsSystem::draw_solid_polygon;
    m_world_debug_draw.DrawSegmentFcn = PhysicsSystem::draw_segment;

    // player
    float screen_width{ (float)GetScreenWidth() };
    float screen_height{ (float)GetScreenWidth() };
    b2Vec2 player_pos{ (b2Vec2){ screen_width / 2.0F, 0 } };
    m_camera_2d = Camera2D{ 0 };
    m_camera_2d.target = Vector2{ .x = player_pos.x, .y = player_pos.y };
    m_camera_2d.offset = (Vector2){ screen_width / 2.0F, screen_height / 2.0F };
    m_camera_2d.rotation = 0.0F;
    m_camera_2d.zoom = 1.0F;

    // enemy
    b2Vec2 enemy_pos{ (b2Vec2){ (screen_width / 2.0F) + 30.0F, 0 } };

    // modules setup
    TileWorldModule::setup(ctx, m_world_id);
    PlayerModule::setup(player_pos, m_world_id, ctx);
    EnemyModule::setup(enemy_pos, m_world_id, ctx);
}

void Scene::GameScene::on_update(GameContext::GameContext& ctx) {
    b2World_Step(m_world_id, constants::TIME_STEP, constants::SUB_STEP_COUNT);

    // TODO: this order is somehow important to state system and animations: Controller -> Attack -> State -> Animation
    ControllerSystem::update(ctx);
    AttackSystem::update(ctx, m_world_id);
    StateSystem::update(ctx);
    MovementSystem::update(ctx);
    PhysicsSystem::update(ctx);
    PhysicsSensorSystem::update(ctx, m_world_id);
    CameraSystem::update(ctx, m_camera_2d);
    AnimationSystem::update(ctx);

    if (IsKeyPressed(constants::DEBUG_KEY)) {
        m_debug_mode = !m_debug_mode;
    }
}


void Scene::GameScene::on_render(GameContext::GameContext& ctx) {
    BeginMode2D(m_camera_2d);
    Render2DSystem::render(ctx);
    if (m_debug_mode) {
        b2World_Draw(m_world_id, &m_world_debug_draw); // draw box2d shapes for debugging
        Render2DSystem::render_raycasts(ctx);
    }

    if (m_debug_mode) {
        ctx.registry
            .system<components::PositionComponent, components::StateComponent, components::BaseColliderComponent>()
            .each([](components::PositionComponent& pos, components::StateComponent& state,
                     components::BaseColliderComponent& base_collider) {
                DrawText(
                    state.curr_state_id.c_str(),
                    (int)(pos.x - (base_collider.width / 2.0F)),
                    (int)(pos.y - (base_collider.height / 2) - 12), 12, WHITE
                ); // here we do - 12 because that is the font-size so font-height too, and we want the text to be anchored at bottom right of this pos
            })
            .run();
    }

    EndMode2D();

    if (m_debug_mode) {
        Interface::game_debug_GUI(ctx);
    }
}

void Scene::GameScene::on_exit(GameContext::GameContext& ctx) {
    b2DestroyWorld(m_world_id);
    ctx.registry.reset();
    rlImGuiShutdown();
}
