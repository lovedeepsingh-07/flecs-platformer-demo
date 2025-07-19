#include "box2d/box2d.h"
#include "box2d/types.h"
#include "constants.hpp"
#include "interface.hpp"
#include "modules.hpp"
#include "rlImGui.h"
#include "screen.hpp"
#include "systems.hpp"

void Screen::GameScreen::on_enter(flecs::world& registry) {
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

    // player
    float screen_width{ (float)GetScreenWidth() };
    float screen_height{ (float)GetScreenWidth() };
    b2Vec2 player_pos{ (b2Vec2){ screen_width / 2.0F, screen_height / 3.0F } };
    m_camera_2d = Camera2D{ 0 };
    m_camera_2d.target = Vector2{ .x = player_pos.x, .y = player_pos.y };
    m_camera_2d.offset = (Vector2){ screen_width / 2.0F, screen_height / 2.0F };
    m_camera_2d.rotation = 0.0F;
    m_camera_2d.zoom = 1.0F;
    Player::setup(player_pos, m_world_id, registry);

    // world
    TileWorld::setup(registry, m_world_id, m_ldtk_project);
}

void Screen::GameScreen::on_update(flecs::world& registry) {
    b2World_Step(m_world_id, constants::TIME_STEP, constants::SUB_STEP_COUNT);
    ControllerSystem::update(registry);
    PhysicsSystem::update(registry);
    CameraSystem::update(registry, m_camera_2d);

    if (IsKeyPressed(constants::DEBUG_KEY)) {
        m_debug_mode = !m_debug_mode;
    }
}


void Screen::GameScreen::on_render(flecs::world& registry) {
    BeginMode2D(m_camera_2d);
    Render2DSystem::update(registry);
    if (m_debug_mode) {
        b2World_Draw(m_world_id, &m_world_debug_draw); // draw box2d shapes for debugging
    }
    EndMode2D();

    if (m_debug_mode) {
        Interface::game_debug_GUI(registry);
    }
}

void Screen::GameScreen::on_exit(flecs::world& registry) {
    b2DestroyWorld(m_world_id);
    registry.reset();
    rlImGuiShutdown();
}
