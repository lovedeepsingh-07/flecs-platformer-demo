#include "box2d/types.h"
#define CLAY_IMPLEMENTATION
#include "box2d/box2d.h"
#include "clay/clay.h"
#include "clay/renderers/raylib/clay_renderer_raylib.c"
#include "raylib.h"
#include <iostream>

class CustomDebugDraw : public b2DebugDraw {
  public:
    void DrawCircleFcn(b2Vec2 center, float radius, b2HexColor color, void *context);
    void DrawPointFcn(b2Vec2 point, float size, b2HexColor color, void *context) {}
    void DrawPolygonFcn(const b2Vec2 *vertices, int vertexCount, b2HexColor color, void *context) {
        std::cout << "shit works!" << "\n";
    }
    void DrawSegmentFcn(b2Vec2 point_1, b2Vec2 point_2, b2HexColor color, void *context) {}
    void DrawSolidCapsuleFcn(b2Vec2 point_1, b2Vec2 point_2, float radius, b2HexColor color, void *context) {
    }
    void DrawSolidCircleFcn(b2Transform transform, float radius, b2HexColor color, void *context) {
    }
    void DrawSolidPolygonFcn(
        b2Transform transform, const b2Vec2 *vertices, int vertex_count, float radius, b2HexColor color, void *context
    ) {}
    void DrawStringFcn(b2Vec2 point, const char *string_value, b2HexColor color, void *context) {
    }
    void DrawTransformFcn(b2Transform transform, void *context) {}
};

void draw_polygon(const b2Vec2 *vertices, int vertexCount, b2HexColor color, void *context) {
    std::cout << "shit works!" << "\n";
}


auto main() -> int {
    Clay_Raylib_Initialize(0, 0, "basement", FLAG_WINDOW_UNDECORATED);
    int screen_width = GetMonitorWidth(0);
    int screen_height = GetMonitorHeight(0);
    SetWindowSize(screen_width, screen_height);
    SetTargetFPS(75);


    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity.y = 9.8F * 32 * 100;
    b2WorldId world_id = b2CreateWorld(&world_def);
    std::cout << "shit starts" << "\n";

    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = b2_dynamicBody;
    body_def.position = (b2Vec2){ (float)screen_width / 2, 10 };
    body_def.fixedRotation = true;
    b2BodyId body_id = b2CreateBody(world_id, &body_def);
    b2Polygon body_polygon = b2MakeBox(32, 32);
    b2ShapeDef body_shape_def = b2DefaultShapeDef();
    body_shape_def.density = 1.0F;
    body_shape_def.material.friction = 0.0F;
    b2CreatePolygonShape(body_id, &body_shape_def, &body_polygon);

    while (!WindowShouldClose()) {
        b2World_Step(world_id, 1 / 60.0F, 4);

        BeginDrawing();
        ClearBackground(BLACK);

        b2Vec2 body_pos = b2Body_GetPosition(body_id);
        // DrawRectangle(body_pos.x, body_pos.y, 32, 32, RAYWHITE);

        b2DebugDraw debug_draw = b2DefaultDebugDraw();
        debug_draw.drawShapes = true;
        debug_draw.DrawPolygonFcn = draw_polygon;
        b2World_Draw(world_id, &debug_draw);

        EndDrawing();
    }

    Clay_Raylib_Close();

    return 0;
}
