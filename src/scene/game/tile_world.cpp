#include "constants.hpp"
#include "scene.hpp"
#include <pugixml.hpp>
#include <rapidcsv.h>
#include <sstream>

void scene::game::setup_tile_world(flecs::world& registry, b2WorldId world_id) {
    flecs::entity scene_root = registry.component<components::SceneRoot>();

    auto& texture_engine = registry.get_mut<components::Texture_Engine>();

    texture_engine.engine.load_texture("tiles_debug", "assets/tiles-debug.png");

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("assets/map.xml");
    if (!result) {
        throw std::runtime_error("Failed to load XML file");
    }
    pugi::xml_node tile_layer =
        doc.child("map").find_child_by_attribute("layer", "name", "DrawingLayer");
    if (tile_layer == nullptr) {
        throw std::runtime_error("Failed to get tile layer");
    }
    std::stringstream tile_layer_stream(tile_layer.child("data").child_value());
    rapidcsv::Document csv_data(tile_layer_stream, rapidcsv::LabelParams(-1, -1));

    float debug_tileset_first_gid =
        std::stof(doc.child("map")
                      .find_child_by_attribute("tileset", "source", "tiles-debug.xml")
                      .attribute("firstgid")
                      .value());

    Texture2D tileset_debug_texture =
        texture_engine.engine.get_texture("tiles_debug");
    int map_width = std::stoi(tile_layer.attribute("width").value());
    int map_height = std::stoi(tile_layer.attribute("height").value());

    for (int grid_y = 0; grid_y < map_height; grid_y++) {
        for (int grid_x = 0; grid_x < map_width; grid_x++) {
            auto gid_value = csv_data.GetCell<float>(grid_x, grid_y);
            if (gid_value != 0) {
                float tile_width = constants::WORLD_SCALE;
                float tile_height = constants::WORLD_SCALE;
                float tile_x = (float)grid_x * tile_width;
                float tile_y = (float)grid_y * tile_height;

                float tile_index = gid_value - debug_tileset_first_gid;
                float tile_rect_x =
                    (float)((int)tile_index % (int)((float)tileset_debug_texture.width / tile_width))
                    * tile_width;
                float tile_rect_y =
                    std::floor(tile_index / ((float)tileset_debug_texture.width / tile_width))
                    * tile_height;

                // tile entity
                registry.entity()
                    .set(components::Position{ tile_x, tile_y })
                    .set(components::TextureComponent{
                        .texture = tileset_debug_texture,
                        .source_rect =
                            (Rectangle){
                                .x = tile_rect_x,
                                .y = tile_rect_y,
                                .width = tile_width,
                                .height = tile_height,
                            },
                        .flipped = false,
                    })
                    .child_of(scene_root);
            };
        }
    }

    pugi::xml_node object_layer =
        doc.child("map").find_child_by_attribute("objectgroup", "name", "GroundLayer");
    if (object_layer == nullptr) {
        throw std::runtime_error("Failed to get object layer");
    }
    for (pugi::xml_node curr_object : object_layer.children()) {
        float object_x = std::stof(curr_object.attribute("x").value());
        float object_y = std::stof(curr_object.attribute("y").value());

        std::vector<b2Vec2> vertices;
        std::stringstream object_points_stream(
            curr_object.child("polygon").attribute("points").value()
        );
        std::string pair;

        while (std::getline(object_points_stream, pair, ' ')) {
            float point_x = 0;
            float point_y = 0;
            char comma = 0;
            std::stringstream pairStream(pair);
            pairStream >> point_x >> comma >> point_y;
            point_x -= constants::WORLD_SCALE / 2;
            point_y -= constants::WORLD_SCALE / 2;
            vertices.emplace_back(b2Vec2{ point_x, point_y });
        }

        b2BodyDef body_def{ b2DefaultBodyDef() };
        body_def.type = b2_staticBody;
        body_def.fixedRotation = true;
        body_def.position = (b2Vec2){ object_x, object_y };
        b2BodyId body_id = b2CreateBody(world_id, &body_def);
        b2ChainDef body_chain_def = b2DefaultChainDef();
        body_chain_def.points = vertices.data();
        body_chain_def.count = (int)vertices.size();
        body_chain_def.isLoop = true;
        body_chain_def.enableSensorEvents = true;
        b2CreateChain(body_id, &body_chain_def);

        registry.entity().set(components::PhysicalBody{ body_id });
    }
}
