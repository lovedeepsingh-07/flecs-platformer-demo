#pragma once

#include "box2d/types.h"
#include "context.hpp"
#include "raylib.h"
#include <cstdint>

class Scene {
  public:
    virtual ~Scene() = default;
    virtual void on_enter(GameContext& ctx) = 0;
    virtual void on_update(GameContext& ctx) = 0;
    virtual void on_render(GameContext& ctx) = 0;
    virtual void on_exit(GameContext& ctx) = 0;
};

class MainMenuScene : public Scene {
  public:
    ~MainMenuScene() override = default;
    void on_enter(GameContext& ctx) override;
    void on_update(GameContext& ctx) override;
    void on_render(GameContext& ctx) override;
    void on_exit(GameContext& ctx) override;
};
class GameScene : public Scene {
  private:
    b2WorldId m_world_id;
    b2DebugDraw m_world_debug_draw;
    Camera2D m_camera_2d;
    bool m_debug_mode{ false };

  public:
    ~GameScene() override = default;
    void on_enter(GameContext& ctx) override;
    void on_update(GameContext& ctx) override;
    void on_render(GameContext& ctx) override;
    void on_exit(GameContext& ctx) override;
};
