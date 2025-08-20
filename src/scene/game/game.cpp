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
    registry.delete_with(flecs::ChildOf, registry.entity<components::SceneRoot>());
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

    scene::game::setup_player(registry, world_id);
    scene::game::setup_enemy(registry, world_id);
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
