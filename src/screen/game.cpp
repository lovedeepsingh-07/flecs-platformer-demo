#include "LDtkLoader/DataTypes.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"
#include "components.hpp"
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
    float screen_width{ (float)GetScreenWidth() };
    float screen_height{ (float)GetScreenWidth() };
    b2Vec2 player_pos{ (b2Vec2){ screen_width / 2.0F, screen_height } };
    b2Vec2 player_size{ (b2Vec2){ constants::PLAYER_WIDTH, constants::PLAYER_HEIGHT } };
    m_camera_2d = Camera2D{ 0 };
    m_camera_2d.target = Vector2{ .x = player_pos.x, .y = player_pos.y };
    m_camera_2d.offset =
        (Vector2){ (float)GetScreenWidth() / 2.0F, (float)GetScreenHeight() / 2.0F };
    m_camera_2d.rotation = 0.0F;
    m_camera_2d.zoom = 1.0F;
    b2BodyDef body_def{ b2DefaultBodyDef() };
    body_def.type = b2_dynamicBody;
    body_def.position = player_pos;
    body_def.fixedRotation = true;
    b2BodyId body_id = b2CreateBody(m_world_id, &body_def);
    b2Polygon body_polygon = b2MakeBox(player_size.x / 2, player_size.y / 2);
    b2ShapeDef body_shape_def = b2DefaultShapeDef();
    body_shape_def.density = 1.0F;
    body_shape_def.material.friction = 0.0F;
    b2CreatePolygonShape(body_id, &body_shape_def, &body_polygon);
    flecs::entity player{
        registry.entity()
            .set(components::PositionComponent{ player_pos.x - (player_size.x / 2),
                                                player_pos.y - (player_size.y / 2) })
            .set(components::SizeComponent{ player_size.x, player_size.y })
            .set(components::PhysicsComponent{ body_id })
            .add<components::ControllerComponent>()
            .add<components::CameraComponent>()
            .set(components::RectangleComponent{ RAYWHITE })
    };

    // world
    m_ldtk_project.loadFromFile("./assets/world.ldtk");

    for (const auto& tile :
         m_ldtk_project.getWorld().getLevel("Level_0").getLayer("IntegerLayer").allTiles()) {
        ldtk::IntPoint tile_pos = tile.getPosition();
        ldtk::IntRect tile_rect = tile.getTextureRect();
        ldtk::IntPoint grid_pos = tile.getGridPosition();
        ldtk::IntGridValue curr_int_val =
            tile.layer->getIntGridVal(grid_pos.x, grid_pos.y);

        flecs::entity tile_entity{
            registry.entity()
                .set(components::PositionComponent{
                    (float)tile_pos.x - ((float)tile_rect.width / 2),
                    (float)tile_pos.y - ((float)tile_rect.height / 2) })
                .set(components::SizeComponent{ (float)tile_rect.width,
                                                (float)tile_rect.height })
        };

        if (curr_int_val.value == 1) {
            float screen_width{ (float)GetScreenWidth() };
            float screen_height{ (float)GetScreenWidth() };
            b2BodyDef body_def{ b2DefaultBodyDef() };
            body_def.type = b2_staticBody;
            body_def.position = (b2Vec2){ (float)tile_pos.x, (float)tile_pos.y };
            body_def.fixedRotation = true;
            b2BodyId body_id = b2CreateBody(m_world_id, &body_def);
            b2Polygon body_polygon =
                b2MakeBox((float)tile_rect.width / 2, (float)tile_rect.height / 2);
            b2ShapeDef body_shape_def = b2DefaultShapeDef();
            body_shape_def.density = 1.0F;
            body_shape_def.material.friction = 0.0F;
            b2CreatePolygonShape(body_id, &body_shape_def, &body_polygon);
            tile_entity.set(components::RectangleComponent{ GRAY }).set(components::PhysicsComponent{ body_id });
        }
        if (curr_int_val.value == 2) {
            tile_entity.set(components::PositionComponent{
                (float)tile_pos.x - ((float)tile_rect.width / 2),
                (float)tile_pos.y - ((float)tile_rect.height / 2) });
        }

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
    }
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
