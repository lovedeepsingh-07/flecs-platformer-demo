#pragma once

#include "clay/clay.h"
#include "context.hpp"
#include "raylib.h"
#include "scene.hpp"
#include <array>
#include <memory>
#include <unordered_map>

class SceneManager {
  private:
    std::unordered_map<std::uint8_t, std::shared_ptr<Scene>> m_scenes;
    std::shared_ptr<Scene> m_current_scene = nullptr;
    std::uint8_t m_current_scene_index;

  public:
    std::array<Font, 1> m_fonts{ GetFontDefault() };

    SceneManager() = default;
    ~SceneManager() = default;
    void init();
    void update(GameContext& ctx);
    void render(GameContext& ctx);
    void add_scene(GameContext& ctx, const std::uint8_t& index, std::shared_ptr<Scene> scene);
    void switch_to(GameContext& ctx, const std::uint8_t& index);
    void shutdown();
};
