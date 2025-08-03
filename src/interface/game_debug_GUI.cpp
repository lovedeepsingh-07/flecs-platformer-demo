#include "components.hpp"
#include "interface.hpp"
#include <imgui.h>
#include <rlImGui.h>

void player_debug(GameContext::GameContext& ctx) {
    flecs::query<> player_debug_display_query =
        ctx.registry.query_builder()
            .with<components::PositionComponent>()
            .with<components::SizeComponent>()
            .with<components::PhysicsComponent>()
            // .with<components::TextureComponent>()
            // .with<components::AnimationComponent>()
            // .with<components::AnimationStatesComponent>()
            // .with<components::AttackComponent>()
            .with<components::MovementComponent>()
            .with<components::ControllerComponent>()
            .with<components::CameraComponent>()
            .build();
    ctx.registry.defer_begin();
    player_debug_display_query.run([](flecs::iter& iter) {
        while (iter.next()) {
            auto pos = iter.field<components::PositionComponent>(0);
            // auto texture = iter.field<components::TextureComponent>(3);
            // auto animation = iter.field<components::AnimationComponent>(4);
            // auto animation_states = iter.field<components::AnimationStatesComponent>(5);
            // auto attack = iter.field<components::AttackComponent>(9);
            auto movement = iter.field<components::MovementComponent>(3);
            for (auto i : iter) {
                flecs::entity curr_entity = iter.entity(i);
                // player debug information
                ImGui::Text("Position: (%.f, %.f)", pos[i].x, pos[i].y);
                ImGui::Text("OnGround: %s", movement[i].on_ground ? "true" : "false");
                ImGui::Text("Jumping: %s", movement[i].jumping ? "true" : "false");
                ImGui::Text("Falling: %s", movement[i].falling ? "true" : "false");
                // ImGui::Text("Curr Frame Index: %d", animation[i].curr_frame_index);
                // ImGui::Text("Curr Animation State: %s", animation[i].curr_state.c_str());
                // ImGui::Text(
                //     "Loop Animation: %s",
                //     animation_states[i].clips[animation[i].curr_state].loop ? "true" : "false"
                // );
                // ImGui::Text("Animation Playing: %s", animation[i].playing ? "true" : "false");
                // ImGui::Text("Animation Finished: %s", animation[i].finished ? "true" : "false");
                // ImGui::Text("Texture Flipped: %s", texture[i].flipped ? "true" : "false");
                // ImGui::Text("Attacking: %s", attack[i].attacking ? "true" : "false");

                // player debug actions
                if (ImGui::Button("Jump Button")
                    && !curr_entity.has<components::JumpEventComponent>()
                    && movement[i].on_ground) {
                    curr_entity.add<components::JumpEventComponent>();
                }
            }
        }
    });
    ctx.registry.defer_end();
}

void Interface::game_debug_GUI(GameContext::GameContext& ctx) {
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
        player_debug(ctx);
    }
    ImGui::End();
    rlImGuiEnd();
}
