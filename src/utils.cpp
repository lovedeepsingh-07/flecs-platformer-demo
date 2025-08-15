#include "utils.hpp"
#include "box2d/box2d.h"
#include <clay/clay.h>
#include <iostream>
#include <raylib.h>

Clay_Color Utils::RaylibColorToClayColor(Color rayColor) {
    Clay_Color clayColor;
    clayColor.r = rayColor.r;
    clayColor.g = rayColor.g;
    clayColor.b = rayColor.b;
    clayColor.a = rayColor.a;
    return clayColor;
}

void Utils::HandleClayErrors(Clay_ErrorData errorData) {
    std::cout << errorData.errorText.chars << "\n";
}

float Utils::cast_result_fcn(b2ShapeId shape_id, b2Vec2 point, b2Vec2 normal, float fraction, void* context) {
    auto* cast_context = static_cast<Utils::CastContext*>(context);
    auto* entity_hit_shape_data =
        static_cast<Utils::ShapeUserData*>(b2Shape_GetUserData(shape_id));
    cast_context->hit = true;
    cast_context->hit_entity = entity_hit_shape_data->_owner;
    return 1;
}
