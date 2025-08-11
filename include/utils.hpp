#pragma once

#include <box2d/box2d.h>
#include <clay/clay.h>
#include <raylib.h>
#include <string>

namespace Utils {

enum EntityCategories : uint8_t {
    PLAYER = 0x00000002,
    ENEMY = 0x00000004,
};

struct ShapeUserData {
    std::string _id;
    void* _data;
};
struct RayCastUserData {
    std::string _id;
    void* _data;
};

float cast_result_fcn(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context);

Clay_Color RaylibColorToClayColor(Color rayColor);
void HandleClayErrors(Clay_ErrorData errorData);

}
