#include "box2d/box2d.h"
#include "components.hpp"
#include "modules.hpp"
#include "raylib.h"

void TileWorld::setup(flecs::world& registry, b2WorldId world_id, ldtk::Project& ldtk_project) {
    ldtk_project.loadFromFile("./assets/world.ldtk");

    for (const auto& tile :
         ldtk_project.getWorld().getLevel("Level_0").getLayer("IntegerLayer").allTiles()) {
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
            b2BodyId body_id = b2CreateBody(world_id, &body_def);
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
