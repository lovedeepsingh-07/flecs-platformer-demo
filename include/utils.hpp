#pragma once

#include <box2d/box2d.h>
#include <clay/clay.h>
#include <flecs.h>
#include <raylib.h>
#include <string>

namespace Utils {

enum EntityCategories : uint8_t {
    GROUND = 0x01,
    PLAYER = 0x02,
    ENEMY = 0x04,
};

// the usual way of naming the ID is: entity_type_category
// entity - player,enemy
// type - shape, sensor, raycast
// category - where that type is, like the "shape" of the "body", or "sensor" of the "ground"
struct ShapeUserData {
    std::string _id;
    flecs::entity _owner;
    void* _data;
};
struct SensorUserData {
    std::string _id;
    flecs::entity _owner;
    void* _data;
};
struct RayCastUserData {
    std::string _id;
    flecs::entity _owner;
    void* _data;
};
struct CastContext {
    void* cast_user_data;
    bool hit;
    flecs::entity hit_entity;
};

float cast_result_fcn(b2ShapeId shape_id, b2Vec2 point, b2Vec2 normal, float fraction, void* context);

Clay_Color RaylibColorToClayColor(Color rayColor);
void HandleClayErrors(Clay_ErrorData errorData);

}
