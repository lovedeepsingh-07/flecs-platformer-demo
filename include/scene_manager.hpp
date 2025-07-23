#pragma once

#include "clay/clay.h"
#include "context.hpp"
#include "flecs.h"
#include "raylib.h"
#include "scene.hpp"
#include <array>
#include <memory>
#include <unordered_map>

namespace SceneManager {

class SceneManager {
  private:
    std::unordered_map<SceneLabel, std::shared_ptr<Scene>> m_screens;
    std::shared_ptr<Scene> m_current_screen = nullptr;
    SceneLabel m_current_screen_label{};

  public:
    std::array<Font, 1> m_fonts{ GetFontDefault() };

    SceneManager() = default;
    ~SceneManager() = default;
    void init();
    void update(GameContext& ctx);
    void render(GameContext& ctx);
    void add_scene(GameContext& ctx, const SceneLabel& label, std::shared_ptr<Scene> screen);
    void switch_to(GameContext& ctx, const SceneLabel& label);
    void shutdown();
};

} // namespace SceneManager
