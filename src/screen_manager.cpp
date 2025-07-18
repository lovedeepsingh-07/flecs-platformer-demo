#include "screen_manager.hpp"
#include "clay/clay.h"
#include "clay/renderers/raylib/clay_renderer_raylib.c"
#include "constants.hpp"
#include "interface.hpp"
#include "raylib.h"
#include "screen.hpp"
#include "utils.hpp"
#include <memory>
#include <utility>

void ScreenManager::ScreenManager::init() {
    // setup raylib window
    Clay_Raylib_Initialize(0, 0, "game", FLAG_WINDOW_UNDECORATED);
    int screen_width = GetMonitorWidth(0);
    int screen_height = GetMonitorHeight(0);
    SetWindowSize(screen_width, screen_height);
    SetTargetFPS(Constants::FPS);

    // setup clay
    uint64_t clay_required_memory = Clay_MinMemorySize();
    auto clay_memory = Clay_Arena{
        .capacity = clay_required_memory,
        .memory = (char*)malloc(clay_required_memory),
    };
    Clay_Initialize(
        clay_memory,
        Clay_Dimensions{
            .width = (float)GetScreenWidth(),
            .height = (float)GetScreenHeight(),
        },
        (Clay_ErrorHandler){ Utils::HandleClayErrors }
    );

    // setup fonts
    Clay_SetMeasureTextFunction(Raylib_MeasureText, m_fonts.data());

    // add screens
    add_screen(Screen::ScreenLabel::MainMenu, std::make_shared<Screen::MainMenuScreen>());
    add_screen(Screen::ScreenLabel::Game, std::make_shared<Screen::GameScreen>());

    // set the default screen to "MainMenu"
    switch_to(Screen::ScreenLabel::MainMenu);
}

void ScreenManager::ScreenManager::update() {
    m_current_screen->on_update(m_registry);
}

void ScreenManager::ScreenManager::render() {
    Clay_SetLayoutDimensions((Clay_Dimensions
    ){ .width = (float)GetScreenWidth(), .height = (float)GetScreenHeight() });
    Vector2 mouse_pos = GetMousePosition();
    Clay_SetPointerState(Clay_Vector2{ mouse_pos.x, mouse_pos.y }, IsMouseButtonDown(0));

    // clay UI
    Clay_BeginLayout();
    if (m_current_screen_label == Screen::ScreenLabel::MainMenu) {
        Interface::main_menu(this);
    }
    if (m_current_screen_label == Screen::ScreenLabel::Game) {
        Interface::game(this);
    }
    Clay_RenderCommandArray render_commands = Clay_EndLayout();

    m_current_screen->on_render(m_registry);
    Clay_Raylib_Render(render_commands, m_fonts.data());
}

void ScreenManager::ScreenManager::add_screen(
    const Screen::ScreenLabel& label, std::shared_ptr<Screen::Screen> screen
) {
    m_screens[label] = std::move(screen);
}

void ScreenManager::ScreenManager::switch_to(const Screen::ScreenLabel& label) {
    if (m_current_screen) {
        m_current_screen->on_exit(m_registry);
    }
    m_current_screen = m_screens[label];
    m_current_screen_label = label;
    if (m_current_screen) {
        m_current_screen->on_enter(m_registry);
    }
}

void ScreenManager::ScreenManager::shutdown() {
    Clay_Raylib_Close();
}
