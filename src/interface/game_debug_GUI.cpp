#include "components.hpp"
#include "interface.hpp"
#include <imgui.h>
#include <rlImGui.h>

void player_debug(GameContext::GameContext& ctx) {
    ctx.registry
        .system<
            components::PositionComponent, components::PhysicsComponent, components::TextureComponent,
            components::AnimationComponent, components::StateRegistryComponent,
            components::StateComponent, components::AttackComponent, components::MovementComponent,
            components::ControllerComponent, components::CameraComponent>()
        .each([&ctx](
                  flecs::entity curr_entity, const components::PositionComponent& pos,
                  const components::PhysicsComponent& phy,
                  const components::TextureComponent& texture,
                  const components::AnimationComponent& animation,
                  const components::StateRegistryComponent& state_registry,
                  const components::StateComponent& state,
                  const components::AttackComponent& attack,
                  const components::MovementComponent& movement,
                  const components::ControllerComponent& controller,
                  const components::CameraComponent& camera
              ) {
            // get current registry
            StateEngine::StateRegistry curr_registry;
            auto state_registry_result =
                ctx.state_engine.get_state_registry(state_registry.state_registry_id);
            if (!state_registry_result) {
                throw std::runtime_error(state_registry_result.error().message);
            }
            curr_registry = *state_registry_result;

            // get current state
            StateEngine::State curr_state;
            auto state_result = curr_registry.get_state(state.curr_state_id);
            if (!state_result) {
                throw std::runtime_error(state_result.error().message);
            }
            curr_state = *state_result;

            b2Vec2 vel = b2Body_GetLinearVelocity(phy.body_id);

            ImGui::Text("Position: (%.f, %.f)", pos.x, pos.y);
            ImGui::Text("OnGround: %s", movement.on_ground ? "true" : "false");
            ImGui::Text("Jumping: %s", movement.jumping ? "true" : "false");
            ImGui::Text("Falling: %s", movement.falling ? "true" : "false");
            ImGui::Text("Left_Idle_Right: %d", movement.left_idle_right);
            ImGui::Text("Velocity_x: %f", vel.x);
            ImGui::Text("Curr State: %s", state.curr_state_id.c_str());
            ImGui::Text("Curr Frame Index: %d", animation.curr_frame_index);
            ImGui::Text(
                "Curr Frame Type: %s",
                curr_state.animation_data.frames[animation.curr_frame_index]
                    ._type.c_str()
            );
            ImGui::Text("Animation Playing: %s", animation.playing ? "true" : "false");
            ImGui::Text("Animation Finished: %s", animation.finished ? "true" : "false");
            ImGui::Text("Texture Flipped: %s", texture.flipped ? "true" : "false");
            ImGui::Text("Attacking: %s", attack.attacking ? "true" : "false");

            // player debug actions
            if (ImGui::Button("Jump Button")
                && !curr_entity.has<components::JumpEventComponent>()
                && movement.on_ground) {
                curr_entity.add<components::JumpEventComponent>();
            }
        })
        .run();
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
