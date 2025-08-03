#include "scene_manager.hpp"
#include "constants.hpp"
#include "interface.hpp"
#include "scene.hpp"
#include "utils.hpp"
#include <clay/clay.h>
#include <clay/renderers/raylib/clay_renderer_raylib.c>
#include <memory>
#include <raylib.h>
#include <utility>

void SceneManager::SceneManager::init() {
    // setup raylib window
    Clay_Raylib_Initialize(0, 0, "game", FLAG_WINDOW_UNDECORATED);
    int screen_width = GetMonitorWidth(0);
    int screen_height = GetMonitorHeight(0);
    SetWindowSize(screen_width, screen_height);
    SetTargetFPS(constants::FPS);

    // setup clay
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
        (Clay_ErrorHandler){ Utils::HandleClayErrors }
    );

    // setup fonts
    Clay_SetMeasureTextFunction(Raylib_MeasureText, m_fonts.data());
}

void SceneManager::SceneManager::update(GameContext::GameContext& ctx) {
    m_current_scene->on_update(ctx);
}

void SceneManager::SceneManager::render(GameContext::GameContext& ctx) {
    Clay_SetLayoutDimensions((Clay_Dimensions
    ){ .width = (float)GetScreenWidth(), .height = (float)GetScreenHeight() });
    Vector2 mouse_pos = GetMousePosition();
    Clay_SetPointerState(Clay_Vector2{ mouse_pos.x, mouse_pos.y }, IsMouseButtonDown(0));

    // clay UI
    Clay_BeginLayout();
    if (m_current_scene_index == 0) {
        Interface::main_menu_GUI(ctx);
    }
    if (m_current_scene_index == 1) {
        Interface::game_GUI(ctx);
    }
    Clay_RenderCommandArray render_commands = Clay_EndLayout();

    m_current_scene->on_render(ctx);
    Clay_Raylib_Render(render_commands, m_fonts.data());
}

void SceneManager::SceneManager::add_scene(
    GameContext::GameContext& ctx, const std::uint8_t& index, std::shared_ptr<Scene::Scene> scene
) {
    m_scenes[index] = std::move(scene);
}

void SceneManager::SceneManager::switch_to(GameContext::GameContext& ctx, const std::uint8_t& index) {
    if (m_current_scene) {
        m_current_scene->on_exit(ctx);
    }
    m_current_scene = m_scenes[index];
    m_current_scene_index = index;
    if (m_current_scene) {
        m_current_scene->on_enter(ctx);
    }
}

void SceneManager::SceneManager::shutdown() {
    Clay_Raylib_Close();
}
