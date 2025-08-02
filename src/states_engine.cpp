#include "state_engine.hpp"
#include <iostream>

void StateEngine::StateEngine::load_state_registry(
    const std::string& registry_id, const std::string& registry_file_path
) {
    YAML::Node states_yaml_root = YAML::LoadFile(registry_file_path);

    const YAML::Node& states_yaml = states_yaml_root["states"];
    if (!states_yaml || !states_yaml.IsSequence()) {
        std::cerr << "Invalid or missing 'states' node." << '\n';
    }

    StateRegistry state_registry;

    for (const YAML::Node& curr_state_yaml : states_yaml) {
        State curr_state;

        for (YAML::const_iterator curr_state_yaml_iter = curr_state_yaml.begin();
             curr_state_yaml_iter != curr_state_yaml.end(); ++curr_state_yaml_iter) {
            // id
            curr_state.id = curr_state_yaml_iter->first.as<std::string>();

            // can_transition_to
            YAML::Node transition_yaml =
                curr_state_yaml_iter->second["can_transition_to"];
            for (const YAML::Node& curr_transition_yaml : transition_yaml) {
                State_can_transition_to curr_transition;
                curr_transition.id =
                    curr_transition_yaml.begin()->first.as<std::string>();
                curr_transition.frame =
                    curr_transition_yaml.begin()->second["frame"].as<int>();
                curr_state.can_transition_to[curr_transition.id] = curr_transition;
            }

            // animation_data
            YAML::Node animation_data_yaml =
                curr_state_yaml_iter->second["animation_data"];
            State_animation_data animation_data;
            animation_data.sprite_sheet =
                animation_data_yaml["sprite_sheet"].as<std::string>();
            YAML::Node frames_yaml = animation_data_yaml["frames"];
            for (const YAML::Node& curr_frame_yaml : frames_yaml) {
                animation_data.frames.emplace_back(Rectangle{
                    curr_frame_yaml[0].as<float>(),
                    curr_frame_yaml[1].as<float>(),
                    curr_frame_yaml[2].as<float>(),
                    curr_frame_yaml[3].as<float>(),
                });
            }
            curr_state.animation_data = animation_data;
        }
        state_registry[curr_state.id] = curr_state;
    }
    this->m_state_registries[registry_id] = state_registry;
}

StateEngine::StateRegistry&
StateEngine::StateEngine::get_state_registry(const std::string& registry_id) {
    return m_state_registries[registry_id];
}
