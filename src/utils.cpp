#include "utils.hpp"
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

float Utils::cast_result_fcn(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context) {
    return 1;
}
