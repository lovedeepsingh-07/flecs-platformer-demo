#pragma once

#include "context.hpp"
#include "scene.hpp"
#include <array>
#include <memory>
#include <unordered_map>

namespace SceneManager {

class SceneManager {
  private:
    std::unordered_map<std::uint8_t, std::shared_ptr<Scene::Scene>> m_scenes;
    std::shared_ptr<Scene::Scene> m_current_scene = nullptr;
    std::uint8_t m_current_scene_index;

  public:
    std::array<Font, 1> m_fonts{ GetFontDefault() };

    SceneManager() = default;
    ~SceneManager() = default;
    void init();
    void update(GameContext::GameContext& ctx);
    void render(GameContext::GameContext& ctx);
    void add_scene(GameContext::GameContext& ctx, const std::uint8_t& index, std::shared_ptr<Scene::Scene> scene);
    void switch_to(GameContext::GameContext& ctx, const std::uint8_t& index);
    void shutdown();
};

};
