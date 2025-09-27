#include "utils.hpp"
#include <iostream>

float game_utils::cast_result_fcn(b2ShapeId shape_id, b2Vec2 point, b2Vec2 normal, float fraction, void* context) {
    auto* cast_context = static_cast<game_utils::CastContext*>(context);
    auto* entity_hit_shape_data =
        static_cast<game_utils::ShapeUserData*>(b2Shape_GetUserData(shape_id));
    cast_context->hit = true;
    cast_context->hit_entity = entity_hit_shape_data->_owner;
    return 1;
}

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

std::string game_utils::key_to_string(int key) {
    switch (key) {
    case 65:
        return "A";
    case 66:
        return "B";
    case 67:
        return "C";
    case 68:
        return "D";
    case 69:
        return "E";
    case 70:
        return "F";
    case 71:
        return "G";
    case 72:
        return "H";
    case 73:
        return "I";
    case 74:
        return "J";
    case 75:
        return "K";
    case 76:
        return "L";
    case 77:
        return "M";
    case 78:
        return "N";
    case 79:
        return "O";
    case 80:
        return "P";
    case 81:
        return "Q";
    case 82:
        return "R";
    case 83:
        return "S";
    case 84:
        return "T";
    case 85:
        return "U";
    case 86:
        return "V";
    case 87:
        return "W";
    case 88:
        return "X";
    case 89:
        return "Y";
    case 90:
        return "Z";
    case 256:
        return "ESCAPE";
    case 257:
        return "ENTER";
    case 259:
        return "BACKSPACE";
    case 263:
        return "LEFT";
    case 262:
        return "RIGHT";
    case 264:
        return "DOWN";
    case 265:
        return "UP";
    // Add more keys as needed...
    default:
        return "UNKNOWN_KEY";
    }
}
