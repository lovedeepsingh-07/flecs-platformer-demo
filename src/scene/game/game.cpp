#include "components.hpp"
#include "constants.hpp"
#include "scene.hpp"
#include "utils.hpp"
#include <box2d/box2d.h>
#include <rlImGui.h>

void scene::game::on_enter(flecs::iter& iter, size_t, components::ActiveScene) {
    flecs::world registry = iter.world();

    // setup raylib + ImGui
    rlImGuiSetup(true);

    // reset the scene
    registry.defer_begin();
    registry.delete_with(flecs::ChildOf, registry.lookup("scene_root"));
    registry.defer_end();

    // setup physical world
    b2SetLengthUnitsPerMeter(constants::WORLD_SCALE);
    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity.y = 9.8F * constants::WORLD_SCALE * constants::FORCE_CONST;
    b2WorldId world_id = b2CreateWorld(&world_def);
    registry.set<components::PhysicalWorld>({ .world_id = world_id });

    // setup physical debug draw
    b2DebugDraw debug_draw = b2DefaultDebugDraw();
    debug_draw.drawShapes = true;
    debug_draw.DrawSolidPolygonFcn = game_utils::draw_solid_polygon;
    debug_draw.DrawSegmentFcn = game_utils::draw_segment;
    registry.set<components::PhysicalDebugDraw>({ debug_draw });

    // setup player
    float screen_width{ (float)GetScreenWidth() };
    float screen_height{ (float)GetScreenWidth() };
    b2Vec2 player_pos{ (b2Vec2){ screen_width / 2.0F, 0 } };
    scene::game::setup_player(registry, world_id, player_pos);

    // setup camera source
    auto cam_source = Camera2D{ 0 };
    cam_source.target = Vector2{ .x = player_pos.x, .y = player_pos.y };
    cam_source.offset = (Vector2){ screen_width / 2.0F, screen_height / 2.0F };
    cam_source.rotation = 0.0F;
    cam_source.zoom = 1.0F;
    registry.set<components::GlobalCamera>({ cam_source });

    scene::game::setup_enemy(registry, world_id, { player_pos.x - 50, player_pos.y });
    scene::game::setup_tile_world(registry, world_id);
}

void scene::game::on_exit(flecs::iter& iter, size_t, components::ActiveScene) {
    flecs::world registry = iter.world();

    // destroy physical world
    auto physical_world = registry.get<components::PhysicalWorld>();
    b2DestroyWorld(physical_world.world_id);
    registry.remove<components::PhysicalWorld>();
    registry.remove<components::PhysicalDebugDraw>();
    if (registry.has<components::DebugMode>()) {
        registry.remove<components::DebugMode>();
    }

    rlImGuiShutdown();
}
