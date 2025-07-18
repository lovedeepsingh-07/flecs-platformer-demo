#include "box2d/box2d.h"
#include "constants.hpp"
#include "imgui.h"
#include "rlImGui.h"
#include "screen.hpp"
#include "systems/camera.hpp"
#include "systems/controller.hpp"
#include "systems/physics.hpp"
#include "systems/render_2d.hpp"

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
    // const auto player{ registry.create() };
    // b2Vec2 player_pos =
    //     (b2Vec2){ Constants::BASE_WINDOW_WIDTH / 2.0F, Constants::BASE_WINDOW_HEIGHT };
    // b2Vec2 player_size = (b2Vec2){ Constants::PLAYER_WIDTH, Constants::PLAYER_HEIGHT };
    //
    // m_camera_2d = Camera2D{ 0 };
    // m_camera_2d.target = Vector2{ .x = player_pos.x, .y = player_pos.y };
    // m_camera_2d.offset =
    //     (Vector2){ (float)GetScreenWidth() / 2.0F, (float)GetScreenHeight() / 2.0F };
    // m_camera_2d.rotation = 0.0F;
    // m_camera_2d.zoom = 1.0F;
    //
    // Utils::create_physical_body(registry, m_world_id, player, player_pos, player_size, b2_dynamicBody);
    // registry.emplace<Components::ControllerComponent>(player);
    // registry.emplace<Components::CameraComponent>(player);
    // registry.emplace<Components::RectangleComponent>(
    //     player, player_size.x, player_size.y, RAYWHITE
    // );

    // world
    // m_ldtk_project.loadFromFile("./assets/world.ldtk");
    //
    // for (const auto& tile :
    //      m_ldtk_project.getWorld().getLevel("Level_0").getLayer("IntegerLayer").allTiles()) {
    //     ldtk::IntPoint tile_pos = tile.getPosition();
    //     ldtk::IntRect tile_rect = tile.getTextureRect();
    //     ldtk::IntPoint grid_pos = tile.getGridPosition();
    //     ldtk::IntGridValue curr_int_val =
    //         tile.layer->getIntGridVal(grid_pos.x, grid_pos.y);
    //
    //     const auto tile_entity{ registry.create() };
    //
    //     if (curr_int_val.value == 1) {
    //         Utils::create_physical_body(
    //             registry, m_world_id, tile_entity,
    //             (b2Vec2){ (float)tile_pos.x, (float)tile_pos.y },
    //             (b2Vec2){ (float)tile_rect.width, (float)tile_rect.height }, b2_staticBody
    //         );
    //         registry.emplace<Components::RectangleComponent>(
    //             tile_entity, tile_rect.width, tile_rect.height, GRAY
    //         );
    //     }
    // if (curr_int_val.value == 2) {
    //     registry.emplace<Components::TransformComponent>(
    //         tile_entity, (float)tile_pos.x - ((float)tile_rect.width /
    //         2), (float)tile_pos.y - ((float)tile_rect.height / 2)
    //     );
    // }

    // registry.emplace<Components::TilesetTextureComponent>(
    //     tile_entity, m_tileset, (float)tile_rect.x, (float)tile_rect.y,
    //     (float)tile_rect.width, (float)tile_rect.height
    // );

    // DEBUG: Different integer values for Integer layers
    // registry.emplace<Components::RectangleLinesComponent>(
    //     tile_entity, tile_rect.width, tile_rect.height,
    //     (Color){ curr_int_val.color.r, curr_int_val.color.g,
    //              curr_int_val.color.g, curr_int_val.color.a }
    // );
    // }
}

void Screen::GameScreen::on_update(flecs::world& registry) {
    b2World_Step(m_world_id, constants::TIME_STEP, constants::SUB_STEP_COUNT);
    ControllerSystem::update(registry);
    PhysicsSystem::update(registry);
    CameraSystem::update(registry, m_camera_2d);

    if (IsKeyPressed(KEY_F10)) {
        m_debug_mode = !m_debug_mode;
    }
}


void Screen::GameScreen::on_render(flecs::world& registry) {
    BeginMode2D(m_camera_2d);
    Render2DSystem::update(registry);
    EndMode2D();

    if (m_debug_mode) {
        b2World_Draw(m_world_id, &m_world_debug_draw);

        rlImGuiBegin();
        bool open = true;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration
            | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav
            | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoMove;

        const float PAD = 10.0F;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos;
        ImVec2 window_pos_pivot;
        window_pos.x = work_pos.x + work_size.x - PAD;
        window_pos.y = work_pos.y + PAD;
        window_pos_pivot.x = 1.0F;
        window_pos_pivot.y = 0.0F;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

        ImGui::SetNextWindowBgAlpha(0.35F);
        if (ImGui::Begin("Overlay Window", &open, window_flags)) {
            ImGui::Text("Testing Overlay Window");
        }
        ImGui::End();
        // GuiLabel((Rectangle){ (float)GetScreenWidth() - 100, 10, 120, 30 }, "DEBUG MODE");
        // GuiMessageBox((Rectangle){ 85, 70, 250, 100 }, "#191#Message Box", "Hi! This is a message!", "Nice;Cool");
        // if (GuiButton((Rectangle){ (float)GetScreenWidth() - 150, 40, 120, 30 }, "jump") != 0) {
        //     auto view{ registry.view<Components::PhysicsComponent, Components::ControllerComponent>() };
        //     view.each([](auto& phy) {
        //         b2Vec2 vel = b2Body_GetLinearVelocity(phy.body_id);
        //
        //         vel.y = -5.0F * Constants::WORLD_SCALE * Constants::FORCE_CONST;
        //         b2Body_SetLinearVelocity(phy.body_id, vel);
        //     });
        // };
        rlImGuiEnd();
    }
}

void Screen::GameScreen::on_exit(flecs::world& registry) {
    b2DestroyWorld(m_world_id);
    registry.reset();
    rlImGuiShutdown();
}
