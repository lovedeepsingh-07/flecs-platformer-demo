#define CLAY_IMPLEMENTATION
#include "components.hpp"
#include "constants.hpp"
#include "observers.hpp"
#include "systems.hpp"
#include "utils.hpp"
#include <iostream>

int main() {
    // raylib + clay setup
    int screen_width = 1280;
    int screen_height = 720;
    Clay_Raylib_Initialize(screen_width, screen_height, "flecs-platformer-demo", FLAG_WINDOW_RESIZABLE);
    // SetWindowSize(screen_width, screen_height);
    SetTargetFPS(constants::TARGET_FPS);
    SetExitKey(0);

    // registry setup
    flecs::world registry;
    registry.set_target_fps(constants::TARGET_FPS);
    registry.entity("scene_root").set_alias("scene_root").add<components::SceneRoot>(); // setup the SceneRoot entity, this will act as the parent of all the entities for a particular scene
    components::setup(registry);
    observers::setup(registry);
    systems::setup(registry);

    // setup per frame memory arena
    registry.set<components::PerFrame_MemoryArena>({ MemoryArena::MemoryArena{} });

    // setup texture_engine
    auto texture_engine = TextureEngine::TextureEngine{};
    texture_engine.setup();
    registry.set<components::Texture_Engine>({ texture_engine });

    // setup state_engine
    auto state_engine = StateEngine::StateEngine{};
    auto state_engine_setup_result = state_engine.setup();
    if (!state_engine_setup_result) {
        std::cerr << state_engine_setup_result.error().message << '\n';
        return 0;
    }
    registry.set<components::State_Engine>({ state_engine });

    // setup controller_engine
    auto controller_engine = ControllerEngine::ControllerEngine{};
    auto controller_engine_setup_result = controller_engine.setup();
    if (!controller_engine_setup_result) {
        std::cerr << controller_engine_setup_result.error().message << '\n';
        return 0;
    }
    registry.set<components::Controller_Engine>({ controller_engine });

    // setup particle_engine
    auto particle_engine = ParticleEngine::ParticleEngine{};
    auto particle_engine_setup_result = particle_engine.setup();
    if (!particle_engine_setup_result) {
        std::cerr << particle_engine_setup_result.error().message << '\n';
        return 0;
    }
    registry.set<components::Particle_Engine>({ particle_engine });

    // clay UI setup
    uint64_t clay_required_memory = Clay_MinMemorySize();
    auto clay_memory = Clay_Arena{
        .capacity = static_cast<size_t>(clay_required_memory),
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
    std::array<Font, 1> font_list{
        LoadFontEx("assets/JetBrainsMonoNLNerdFontComplete-Regular.ttf", 48, NULL, 400)
    };
    SetTextureFilter(font_list[0].texture, TEXTURE_FILTER_BILINEAR);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, font_list.data());
    registry.set<components::global_options::GameFonts>({ font_list });

    // switch ActiveScene to MainMenu
    registry.add<components::ActiveScene, components::scenes::MainMenu>();

    // run the main loop
    registry.app().enable_stats().enable_rest().run();

    Clay_Raylib_Close();
    return 0;
}
