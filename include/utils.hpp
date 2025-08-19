#pragma once

#include <box2d/box2d.h>
#include <clay/clay.h>
#include <raylib.h>

namespace game_utils {

Clay_Color RaylibColorToClayColor(Color rayColor);
void HandleClayErrors(Clay_ErrorData errorData);
void draw_solid_polygon(
    b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* context
);
void draw_segment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context);

}
