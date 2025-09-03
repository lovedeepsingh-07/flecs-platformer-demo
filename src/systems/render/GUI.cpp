#include "interface.hpp"
#include "systems.hpp"

void systems::render::GUI(flecs::world& registry) {
    registry.system("Render Debug GUI")
        .kind(flecs::PreStore)
        .run([](flecs::iter& iter) {
            flecs::world registry = iter.world();

            // after this point, we don't need the camera because after this point it's just the UI that we will be rendering
            if (registry.has<components::ActiveScene, components::scenes::Game>()
                && registry.has<components::GlobalCamera>()) {
                EndMode2D();
            }

            if (registry.has<components::ActiveScene, components::scenes::Game>()
                && registry.has<components::global_options::DebugMode>()) {
                Interface::game_debug_GUI(registry);
            }
        })
        .add<components::system_types::Render>();

    registry.system("Render GUI")
        .kind(flecs::OnStore)
        .run([](flecs::iter& iter) {
            flecs::world registry = iter.world();
            auto game_fonts = registry.get<components::global_options::GameFonts>();

            Clay_SetLayoutDimensions((Clay_Dimensions
            ){ .width = (float)GetScreenWidth(), .height = (float)GetScreenHeight() });
            Vector2 mouse_pos = GetMousePosition();
            Clay_SetPointerState(Clay_Vector2{ mouse_pos.x, mouse_pos.y }, IsMouseButtonDown(0));
            Clay_BeginLayout();
            if (registry.has<components::ActiveScene, components::scenes::MainMenu>()) {
                Interface::main_menu_GUI(registry);
            }
            if (registry.has<components::ActiveScene, components::scenes::Setting>()) {
                Interface::setting_GUI(registry);
            }
            if (registry.has<components::ActiveScene, components::scenes::GameSetup>()) {
                Interface::game_setup_GUI(registry);
            }
            if (registry.has<components::ActiveScene, components::scenes::Game>()) {
                Interface::game_GUI(registry);
            }
            Clay_RenderCommandArray render_commands = Clay_EndLayout();
            Clay_Raylib_Render(render_commands, game_fonts.font_list.data());
        })
        .add<components::system_types::Render>();
}
