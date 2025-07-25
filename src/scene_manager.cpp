#include "scene_manager.hpp"
#include "clay/clay.h"
#include "clay/renderers/raylib/clay_renderer_raylib.c"
#include "constants.hpp"
#include "interface.hpp"
#include "raylib.h"
#include "scene.hpp"
#include "utils.hpp"
#include <memory>
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

void SceneManager::SceneManager::update(GameContext& ctx) {
    m_current_screen->on_update(ctx.registry);
}

void SceneManager::SceneManager::render(GameContext& ctx) {
    Clay_SetLayoutDimensions((Clay_Dimensions
    ){ .width = (float)GetScreenWidth(), .height = (float)GetScreenHeight() });
    Vector2 mouse_pos = GetMousePosition();
    Clay_SetPointerState(Clay_Vector2{ mouse_pos.x, mouse_pos.y }, IsMouseButtonDown(0));

    // clay UI
    Clay_BeginLayout();
    if (m_current_screen_label == SceneLabel::MainMenu) {
        Interface::main_menu_GUI(ctx);
    }
    if (m_current_screen_label == SceneLabel::Game) {
        Interface::game_GUI(ctx);
    }
    Clay_RenderCommandArray render_commands = Clay_EndLayout();

    m_current_screen->on_render(ctx.registry);
    Clay_Raylib_Render(render_commands, m_fonts.data());
}

void SceneManager::SceneManager::add_scene(
    GameContext& ctx, const SceneLabel& label, std::shared_ptr<Scene> screen
) {
    m_screens[label] = std::move(screen);
}

void SceneManager::SceneManager::switch_to(GameContext& ctx, const SceneLabel& label) {
    if (m_current_screen) {
        m_current_screen->on_exit(ctx.registry);
    }
    m_current_screen = m_screens[label];
    m_current_screen_label = label;
    if (m_current_screen) {
        m_current_screen->on_enter(ctx.registry);
    }
}

void SceneManager::SceneManager::shutdown() {
    Clay_Raylib_Close();
}
