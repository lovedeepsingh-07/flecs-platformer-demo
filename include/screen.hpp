#pragma once

#include "LDtkLoader/Project.hpp"
#include "box2d/types.h"
#include "flecs.h"
#include "raylib.h"

namespace Screen {

// uint8_t is used here to ensure efficient use of space
enum ScreenLabel : std::uint8_t { MainMenu, Game };

class Screen {
  public:
    virtual ~Screen() = default;
    virtual void on_enter(flecs::world& registry) = 0;
    virtual void on_update(flecs::world& registry) = 0;
    virtual void on_render(flecs::world& registry) = 0;
    virtual void on_exit(flecs::world& registry) = 0;
};

class MainMenuScreen : public Screen {
  public:
    ~MainMenuScreen() override = default;
    void on_enter(flecs::world& registry) override;
    void on_update(flecs::world& registry) override;
    void on_render(flecs::world& registry) override;
    void on_exit(flecs::world& registry) override;
};
class GameScreen : public Screen {
  private:
    b2WorldId m_world_id;
    b2DebugDraw m_world_debug_draw;
    Camera2D m_camera_2d;
    ldtk::Project m_ldtk_project;
    bool m_debug_mode{ false };

  public:
    ~GameScreen() override = default;
    void on_enter(flecs::world& registry) override;
    void on_update(flecs::world& registry) override;
    void on_render(flecs::world& registry) override;
    void on_exit(flecs::world& registry) override;
};

}; // namespace Screen
