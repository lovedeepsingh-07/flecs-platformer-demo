#include "box2d/box2d.h"
#include "components.hpp"
#include "constants.hpp"
#include "imgui.h"
#include "interface.hpp"
#include "rlImGui.h"

void Interface::game_debug_GUI(flecs::world& registry) {
    rlImGuiBegin();
    bool open = true;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav
        | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoMove;

    const float PAD = 10.0F;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos;
    ImVec2 window_pos_pivot;
    window_pos.x = work_pos.x + work_size.x - PAD;
    window_pos.y = work_pos.y + PAD;
    window_pos_pivot.x = 1.0F;
    window_pos_pivot.y = 0.0F;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

    ImGui::SetNextWindowBgAlpha(0.35F);
    if (ImGui::Begin("Overlay Window", &open, window_flags)) {
        ImGui::Text("Testing Overlay Window");
        if (ImGui::Button("Jump Button")) {
            flecs::system button_sys =
                registry
                    .system<components::PhysicsComponent, components::ControllerComponent>()
                    .each([](const components::PhysicsComponent& phy,
                             const components::ControllerComponent& controller) {
                        b2Vec2 vel = b2Body_GetLinearVelocity(phy.body_id);

                        vel.y = -5.0F * constants::WORLD_SCALE * constants::FORCE_CONST;
                        b2Body_SetLinearVelocity(phy.body_id, vel);
                    });
            button_sys.run();
        }
    }
    ImGui::End();
    rlImGuiEnd();
}
