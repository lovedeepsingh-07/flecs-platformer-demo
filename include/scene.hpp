#pragma once

#include "context.hpp"
#include "raylib.h"
#include <box2d/box2d.h>

namespace Scene {

class Scene {
  public:
    virtual ~Scene() = default;
    virtual void on_enter(GameContext::GameContext& ctx) = 0;
    virtual void on_update(GameContext::GameContext& ctx) = 0;
    virtual void on_render(GameContext::GameContext& ctx) = 0;
    virtual void on_exit(GameContext::GameContext& ctx) = 0;
};

class MainMenuScene : public Scene {
  public:
    ~MainMenuScene() override = default;
    void on_enter(GameContext::GameContext& ctx) override;
    void on_update(GameContext::GameContext& ctx) override;
    void on_render(GameContext::GameContext& ctx) override;
    void on_exit(GameContext::GameContext& ctx) override;
};
class GameScene : public Scene {
  private:
    b2WorldId m_world_id;
    b2DebugDraw m_world_debug_draw;
    Camera2D m_camera_2d;
    bool m_debug_mode{ false };

  public:
    ~GameScene() override = default;
    void on_enter(GameContext::GameContext& ctx) override;
    void on_update(GameContext::GameContext& ctx) override;
    void on_render(GameContext::GameContext& ctx) override;
    void on_exit(GameContext::GameContext& ctx) override;
};

}
