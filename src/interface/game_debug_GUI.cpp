#include "components.hpp"
#include "interface.hpp"
#include "raylib.h"
#include <imgui.h>
#include <rlImGui.h>

void entity_debug(flecs::world& registry, const std::string& entity_id) {
    const auto& state_engine = registry.get<components::State_Engine>();

    flecs::entity curr_entity = registry.lookup(entity_id.c_str());
    const auto& pos = curr_entity.get_mut<components::Position>();
    const auto& movement = curr_entity.get<components::Movement>();
    const auto& body = curr_entity.get<components::PhysicalBody>();
    b2Vec2 vel = b2Body_GetLinearVelocity(body.body_id);
    const auto& texture = curr_entity.get<components::TextureComponent>();
    const auto& state = curr_entity.get<components::State>();
    const auto& animation = curr_entity.get<components::Animation>();

    auto state_registry_result =
        state_engine.engine.get_state_registry(state.state_registry_id);
    if (!state_registry_result) {
        throw std::runtime_error(state_registry_result.error().message);
    }
    const StateEngine::StateRegistry& curr_registry = state_registry_result->get();

    // get current state
    auto state_result = curr_registry.get_state(state.curr_state_id);
    if (!state_result) {
        throw std::runtime_error(state_result.error().message);
    }
    const StateEngine::State& curr_state = state_result->get();

    ImGui::Text("Position: (%.f, %.f)", pos.x, pos.y);
    ImGui::Text("OnGround: %s", movement.on_ground ? "true" : "false");
    ImGui::Text("Jumping: %s", curr_entity.target<components::Jump_Entity>().is_valid() ? "true" : "false");
    ImGui::Text("Dashing: %s", curr_entity.target<components::Dash_Entity>().is_valid() ? "true" : "false");
    ImGui::Text("Left_Idle_Right: %d", movement.left_idle_right);
    ImGui::Text("Velocity_X: %f", vel.x);
    ImGui::Text("Velocity_Y: %f", vel.y);
    ImGui::Text("Curr State: %s", state.curr_state_id.c_str());
    ImGui::Text("Curr Frame Index: %d", animation.curr_frame_index);
    ImGui::Text(
        "Curr Frame Type: %s",
        curr_state.animation_data.frames[animation.curr_frame_index]._type.c_str()
    );
    ImGui::Text("Animation Playing: %s", animation.playing ? "true" : "false");
    ImGui::Text("Texture Flipped: %s", texture.flipped ? "true" : "false");
    ImGui::Text("Attacking: %s", curr_entity.target<components::Attack_Entity>().is_valid() ? "true" : "false");
    ImGui::Text("Buffered Jump: %s", curr_entity.has<components::events::BufferedJumpEvent>() ? "true" : "false");
    ImGui::Text("Buffered Dash: %s", curr_entity.has<components::events::BufferedDashEvent>() ? "true" : "false");
}

void Interface::game_debug_GUI(flecs::world& registry) {
    rlImGuiBegin();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoFocusOnAppearing;

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
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Once, window_pos_pivot);
    ImGui::SetNextWindowBgAlpha(0.35F);
    if (ImGui::Begin("Player Information", &window_1_open, window_flags)) {
        entity_debug(registry, "player");
    }
    ImGui::End();

    bool window_2_open = true;
    window_pos.y = work_size.y - PAD;
    window_pos_pivot.y = 1.0F;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Once, window_pos_pivot);
    if (ImGui::Begin("Enemy Information", &window_2_open, window_flags)) {
        entity_debug(registry, "enemy");
    }
    ImGui::End();

    bool window_3_open = true;
    window_pos = { PAD, PAD };
    window_pos_pivot = { 0.0F, 0.0F };
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Once, window_pos_pivot);
    if (ImGui::Begin("Global Information", &window_3_open, window_flags)) {
        ImGui::Text("FPS: %d", GetFPS());
    }
    ImGui::End();

    rlImGuiEnd();
}
