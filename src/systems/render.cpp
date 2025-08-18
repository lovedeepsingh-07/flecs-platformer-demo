#include "components.hpp"
#include "constants.hpp"
#include "interface.hpp"
#include "systems.hpp"

void systems::render(flecs::world& registry) {
    registry.system<>("Prepare Frame").kind(flecs::PreUpdate).each([]() {
        BeginDrawing();
        ClearBackground(constants::BACKGROUND_COLOR);
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

    registry.system<>("Finish Frame").kind(flecs::OnStore).each([]() {
        EndDrawing();
    });
};
