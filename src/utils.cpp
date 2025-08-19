#include "utils.hpp"
#include <iostream>

Clay_Color game_utils::RaylibColorToClayColor(Color rayColor) {
    Clay_Color clayColor;
    clayColor.r = rayColor.r;
    clayColor.g = rayColor.g;
    clayColor.b = rayColor.b;
    clayColor.a = rayColor.a;
    return clayColor;
}

void game_utils::HandleClayErrors(Clay_ErrorData errorData) {
    std::cout << errorData.errorText.chars << "\n";
}

void game_utils::draw_solid_polygon(
    b2Transform transform, const b2Vec2* vertices, int vertexCount, float radius, b2HexColor color, void* context
) {
    for (int i = 0; i < vertexCount; i++) {
        b2Vec2 curr_vertex = b2TransformPoint(transform, vertices[i]);
        b2Vec2 next_vertex = b2TransformPoint(transform, vertices[(i + 1) % vertexCount]);

        Vector2 p0 = { curr_vertex.x, curr_vertex.y };
        Vector2 p1 = { next_vertex.x, next_vertex.y };

        DrawLineV(p0, p1, GREEN);
    }
}

void game_utils::draw_segment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void* context) {
    DrawLineV((Vector2){ p1.x, p1.y }, (Vector2){ p2.x, p2.y }, GREEN);
};
