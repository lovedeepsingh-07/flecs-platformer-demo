#pragma once

#include "clay/clay.h"
#include "flecs.h"
#include "raylib.h"
#include "screen.hpp"
#include <array>
#include <memory>
#include <unordered_map>

namespace ScreenManager {

class ScreenManager {
  private:
    std::unordered_map<Screen::ScreenLabel, std::shared_ptr<Screen::Screen>> m_screens;
    std::shared_ptr<Screen::Screen> m_current_screen = nullptr;
    Screen::ScreenLabel m_current_screen_label{};
    flecs::world m_registry;
    std::array<Font, 1> m_fonts{ GetFontDefault() };

  public:
    bool m_should_quit_game{ false };
    ScreenManager() = default;
    ~ScreenManager() = default;
    void init();
    void update();
    void render();
    void add_screen(const Screen::ScreenLabel& label, std::shared_ptr<Screen::Screen> screen);
    void switch_to(const Screen::ScreenLabel& label);
    static void shutdown();
};

} // namespace ScreenManager
