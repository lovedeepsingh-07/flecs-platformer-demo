#include "components.hpp"
#include "interface.hpp"
#include <imgui.h>
#include <rlImGui.h>

void entity_debug(flecs::world& registry, const std::string& entity_id) {
    flecs::entity player = registry.lookup(entity_id.c_str());
    auto& pos = player.get_mut<components::Position>();
    auto movement = player.get<components::Movement>();
    auto body = player.get<components::PhysicalBody>();
    b2Vec2 vel = b2Body_GetLinearVelocity(body.body_id);
    auto texture = player.get<components::TextureComponent>();

    ImGui::Text("Position: (%.f, %.f)", pos.x, pos.y);
    ImGui::Text("OnGround: %s", movement.on_ground ? "true" : "false");
    ImGui::Text("Jumping: %s", player.has<components::JumpEvent>() ? "true" : "false");
    ImGui::Text("Left_Idle_Right: %d", movement.left_idle_right);
    ImGui::Text("Velocity_X: %f", vel.x);
    ImGui::Text("Velocity_Y: %f", vel.y);
    // ImGui::Text("Curr State: %s", state.curr_state_id.c_str());
    // ImGui::Text("Curr Frame Index: %d", animation.curr_frame_index);
    // ImGui::Text(
    //     "Curr Frame Type: %s",
    //     curr_state.animation_data.frames[animation.curr_frame_index]._type.c_str()
    // );
    // ImGui::Text("Animation Playing: %s", animation.playing ? "true" : "false");
    // ImGui::Text("Animation Finished: %s", animation.finished ? "true" : "false");
    ImGui::Text("Texture Flipped: %s", texture.flipped ? "true" : "false");
    // ImGui::Text("Attacking: %s", attack.attacking ? "true" : "false");
}

void Interface::game_debug_GUI(flecs::world& registry) {
    rlImGuiBegin();
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

    bool window_1_open = true;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowBgAlpha(0.35F);
    if (ImGui::Begin("Player Information", &window_1_open, window_flags)) {
        entity_debug(registry, "player");
    }
    ImGui::End();

    bool window_2_open = true;
    ImVec2 new_window_pos = window_pos;
    new_window_pos.x = PAD + 100;
    ImGui::SetNextWindowPos(new_window_pos, ImGuiCond_Always, window_pos_pivot);
    if (ImGui::Begin("Enemy Information", &window_2_open, window_flags)) {
        entity_debug(registry, "enemy");
    }
    ImGui::End();
    rlImGuiEnd();
}
