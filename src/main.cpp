#define CLAY_IMPLEMENTATION
#include "components.hpp"
#include "constants.hpp"
#include "observers.hpp"
#include "systems.hpp"
#include "utils.hpp"

int main() {
    // raylib + clay setup
    Clay_Raylib_Initialize(0, 0, "game", FLAG_WINDOW_UNDECORATED);
    int screen_width = GetMonitorWidth(0);
    int screen_height = GetMonitorHeight(0);
    SetWindowSize(screen_width, screen_height);
    SetTargetFPS(constants::TARGET_FPS);
    SetExitKey(0);

    // registry setup
    flecs::world registry;
    registry.set_target_fps(constants::TARGET_FPS);
    components::setup(registry);
    observers::setup(registry);
    systems::setup(registry);

    // steup engines
    registry.set<components::Texture_Engine>({});

    // clay UI setup
    uint64_t clay_required_memory = Clay_MinMemorySize();
    auto clay_memory = Clay_Arena{
        .capacity = clay_required_memory,
        .memory = (char*)malloc(clay_required_memory),
    };
    Clay_Initialize(
        clay_memory,
        Clay_Dimensions{
            .width = (float)screen_width,
            .height = (float)screen_height,
        },
        (Clay_ErrorHandler){ game_utils::HandleClayErrors }
    );

    // setup fonts
    std::array<Font, 1> font_list{ GetFontDefault() };
    Clay_SetMeasureTextFunction(Raylib_MeasureText, font_list.data());
    registry.set<components::GameFonts>({ font_list });

    // switch ActiveScene to MainMenu
    registry.add<components::ActiveScene, components::MainMenu_Scene>();

    // run the main loop
    registry.app().enable_stats().enable_rest().run();

    Clay_Raylib_Close();
    return 0;
}
