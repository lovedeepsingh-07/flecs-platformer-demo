#include "box2d/box2d.h"
#include "components.hpp"
#include "constants.hpp"
#include "imgui.h"
#include "interface.hpp"
#include "rlImGui.h"

void Interface::game_debug_GUI(flecs::world& registry) {
    rlImGuiBegin();
    bool open = true;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav
        | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoMove;

    const float PAD = 20.0F;
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
    if (ImGui::Begin("Player Information", &open, window_flags)) {
        flecs::system player_debug_display_sys =
            registry
                .system<
                    components::PositionComponent, components::SizeComponent, components::PhysicsComponent,
                    components::GroundSensorComponent, components::ControllerComponent, components::CameraComponent>()
                .each([&registry](
                          const components::PositionComponent& pos,
                          const components::SizeComponent& size,
                          const components::PhysicsComponent& phy,
                          const components::GroundSensorComponent& ground_sensor,
                          const components::ControllerComponent& controller,
                          const components::CameraComponent& cam
                      ) {
                    // player debug information
                    ImGui::Text("Position: (%.f, %.f)", pos.x, pos.y);
                    ImGui::Text("OnGround: %s", ground_sensor.on_ground ? "true" : "false");

                    // player debug actions
                    if (ImGui::Button("Jump Button")) {
                        b2Vec2 vel = b2Body_GetLinearVelocity(phy.body_id);

                        vel.y = constants::PLAYER_JUMP_VEL;
                        b2Body_SetLinearVelocity(phy.body_id, vel);
                    }
                });
        player_debug_display_sys.run();
    }
    ImGui::End();
    rlImGuiEnd();
}
