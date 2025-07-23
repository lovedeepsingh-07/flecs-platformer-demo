#pragma once

#include "box2d/types.h"
#include "flecs.h"
#include "raylib.h"
#include <cstdint>

// uint8_t is used here to ensure efficient use of space
enum SceneLabel : std::uint8_t { MainMenu, Game };

class Scene {
  public:
    virtual ~Scene() = default;
    virtual void on_enter(flecs::world& registry) = 0;
    virtual void on_update(flecs::world& registry) = 0;
    virtual void on_render(flecs::world& registry) = 0;
    virtual void on_exit(flecs::world& registry) = 0;
};

class MainMenuScene : public Scene {
  public:
    ~MainMenuScene() override = default;
    void on_enter(flecs::world& registry) override;
    void on_update(flecs::world& registry) override;
    void on_render(flecs::world& registry) override;
    void on_exit(flecs::world& registry) override;
};
class GameScene : public Scene {
  private:
    b2WorldId m_world_id;
    b2DebugDraw m_world_debug_draw;
    Camera2D m_camera_2d;
    bool m_debug_mode{ false };

  public:
    ~GameScene() override = default;
    void on_enter(flecs::world& registry) override;
    void on_update(flecs::world& registry) override;
    void on_render(flecs::world& registry) override;
    void on_exit(flecs::world& registry) override;
};
