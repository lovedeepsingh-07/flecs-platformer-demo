#include "components.hpp"
#include "constants.hpp"
#include "interface.hpp"
#include "raylib.h"
#include "systems.hpp"

void systems::render(flecs::world& registry) {
    registry.system("Prepare Frame").kind(flecs::PreUpdate).run([](flecs::iter& iteer) {
        BeginDrawing();
        ClearBackground(constants::BACKGROUND_COLOR);
    });

    registry
        .system<components::TextureComponent, components::Position>("Render "
                                                                    "Textures")
        .kind(flecs::PreStore)
        .each([](const components::TextureComponent& texture, const components::Position& pos) {
            float source_width = texture.flipped ? -texture.source_rect.width
                                                 : texture.source_rect.width;
            DrawTexturePro(
                texture.texture,
                (Rectangle){ texture.source_rect.x, texture.source_rect.y,
                             source_width, texture.source_rect.height },
                (Rectangle){ pos.x - (texture.source_rect.width / 2),
                             pos.y - (texture.source_rect.height / 2),
                             texture.source_rect.width, texture.source_rect.height },
                (Vector2){ 0, 0 }, 0.0F, WHITE
            );
        });

    registry
        .system<components::RectangleComponent, components::Position>(
            "Render Rectangles"
        )
        .kind(flecs::PreStore)
        .each([](const components::RectangleComponent& rect, const components::Position& pos) {
            DrawRectangle((int)pos.x, (int)pos.y, (int)rect.width, (int)rect.height, rect.color);
        });

    registry.system("Render Physical Hitboxes").kind(flecs::PreStore).run([](flecs::iter& iter) {
        flecs::world registry = iter.world();
        if (!registry.has<components::DebugMode>()
            || !registry.has<components::PhysicalWorld>()
            || !registry.has<components::ActiveScene, components::Game_Scene>()
            || !registry.has<components::PhysicalDebugDraw>()) {
            return;
        }

        auto physical_world = registry.get<components::PhysicalWorld>();
        auto debug_draw = registry.get<components::PhysicalDebugDraw>();
        b2World_Draw(physical_world.world_id, &debug_draw.debug_draw);
    });

    registry.system("Render Debug GUI").kind(flecs::PreStore).run([](flecs::iter& iter) {
        flecs::world registry = iter.world();
        if (registry.has<components::ActiveScene, components::Game_Scene>()
            && registry.has<components::DebugMode>()) {
            Interface::game_debug_GUI(registry);
        }
    });
    registry.system("Render GUI").kind(flecs::PreStore).run([](flecs::iter& iter) {
        flecs::world registry = iter.world();
        auto game_fonts = registry.get<components::GameFonts>();

        Clay_SetLayoutDimensions((Clay_Dimensions
        ){ .width = (float)GetScreenWidth(), .height = (float)GetScreenHeight() });
        Vector2 mouse_pos = GetMousePosition();
        Clay_SetPointerState(Clay_Vector2{ mouse_pos.x, mouse_pos.y }, IsMouseButtonDown(0));
        Clay_BeginLayout();
        if (registry.has<components::ActiveScene, components::MainMenu_Scene>()) {
            Interface::main_menu_GUI(registry);
        }
        if (registry.has<components::ActiveScene, components::Game_Scene>()) {
            Interface::game_GUI(registry);
        }
        Clay_RenderCommandArray render_commands = Clay_EndLayout();
        Clay_Raylib_Render(render_commands, game_fonts.font_list.data());
    });

    registry.system("Finish Frame").kind(flecs::OnStore).run([](flecs::iter& iter) {
        EndDrawing();
    });
};
