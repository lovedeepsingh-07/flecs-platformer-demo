#include "state_engine.hpp"

tl::expected<void, error::StateEngineError> StateEngine::StateEngine::setup() {
    auto registry_load_result =
        this->load_state_registry("player", "data/player.states.yaml");
    if (!registry_load_result) {
        return tl::unexpected(registry_load_result.error());
    }

    registry_load_result = this->load_state_registry("enemy", "data/enemy.states.yaml");
    if (!registry_load_result) {
        return tl::unexpected(registry_load_result.error());
    }

    return {};
}

tl::expected<void, error::StateEngineError>
StateEngine::StateEngine::load_state_registry(const std::string& registry_id, const std::string& registry_file_path) {
    // check if a registry with the same ID already exists
    const auto query_result = this->get_state_registry(registry_id);
    if (query_result) {
        return tl::unexpected(error::StateEngineError{
            error::StateEngineError::Type::StateRegistry_LoadError,
            "registry with this ID already exists" });
    }

    YAML::Node states_yaml_root = YAML::LoadFile(registry_file_path);
    const YAML::Node& states_yaml = states_yaml_root["states"];

    if (!states_yaml || !states_yaml.IsSequence()) {
        return tl::unexpected(error::StateEngineError{
            error::StateEngineError::Type::StateRegistry_LoadError,
            "unable to find 'states' node in YAML" });
    }

    StateRegistry state_registry;

    for (const YAML::Node& curr_state_yaml : states_yaml) {
        State curr_state;

        for (YAML::const_iterator curr_state_yaml_iter = curr_state_yaml.begin();
             curr_state_yaml_iter != curr_state_yaml.end(); ++curr_state_yaml_iter) {
            // id
            curr_state.id = curr_state_yaml_iter->first.as<std::string>();

            // hurtbox
            YAML::Node hurtbox_yaml = curr_state_yaml_iter->second["hurtbox"];
            if (!hurtbox_yaml) {
                curr_state.offensive = false;
            } else {
                curr_state.offensive = true;
                curr_state.hurtbox = Rectangle{
                    hurtbox_yaml[0].as<float>(),
                    hurtbox_yaml[1].as<float>(),
                    hurtbox_yaml[2].as<float>(),
                    hurtbox_yaml[3].as<float>(),
                };
            }

            // can_transition_to
            YAML::Node transition_yaml =
                curr_state_yaml_iter->second["can_transition_to"];
            for (const YAML::Node& curr_transition_yaml : transition_yaml) {
                State_can_transition_to curr_transition;
                curr_transition.id =
                    curr_transition_yaml.begin()->first.as<std::string>();
                curr_transition.frame =
                    curr_transition_yaml.begin()->second["frame"].as<int>();
                const auto transition_load_result =
                    curr_state.load_transition(curr_transition.id, curr_transition);
                if (!transition_load_result) {
                    return tl::unexpected(error::StateEngineError{
                        error::StateEngineError::Type::StateRegistry_LoadError,
                        transition_load_result.error().message });
                }
            }

            // animation_data
            YAML::Node animation_data_yaml =
                curr_state_yaml_iter->second["animation_data"];
            State_animation_data animation_data;
            animation_data.texture_id =
                animation_data_yaml["texture_id"].as<std::string>();
            animation_data.loop = animation_data_yaml["loop"].as<bool>();
            YAML::Node frames_yaml = animation_data_yaml["frames"];
            for (const YAML::Node& curr_frame_yaml : frames_yaml) {
                animation_data.frames.emplace_back(AnimationFrame{
                    ._type = curr_frame_yaml["type"].as<std::string>(),
                    .source_rect = Rectangle{
                        curr_frame_yaml["source_rect"][0].as<float>(),
                        curr_frame_yaml["source_rect"][1].as<float>(),
                        curr_frame_yaml["source_rect"][2].as<float>(),
                        curr_frame_yaml["source_rect"][3].as<float>(),
                    } });
            }
            curr_state.animation_data = animation_data;
        }

        const auto state_load_result = state_registry.load_state(curr_state.id, curr_state);
        if (!state_load_result) {
            return tl::unexpected(error::StateEngineError{
                error::StateEngineError::Type::StateRegistry_LoadError,
                state_load_result.error().message });
        }
    }

    this->m_state_registries.emplace(registry_id, state_registry);
    return {};
}

tl::expected<std::reference_wrapper<const StateEngine::StateRegistry>, error::StateEngineError>
StateEngine::StateEngine::get_state_registry(const std::string& registry_id) const {
    // check if a registry with this ID exists
    auto iter = this->m_state_registries.find(registry_id);
    if (iter == this->m_state_registries.end()) {
        return tl::unexpected(error::StateEngineError{
            error::StateEngineError::Type::StateRegistry_GetError,
            "registry with this ID does not exist" });
    }

    return iter->second;
}

tl::expected<void, error::StateEngineError>
StateEngine::StateRegistry::load_state(const std::string& state_id, const State& state) {
    // check if a state with the same ID already exists
    const auto query_result = this->get_state(state_id);
    if (query_result) {
        return tl::unexpected(error::StateEngineError{
            error::StateEngineError::Type::State_LoadError, "state with this ID already exists" });
    }

    this->m_states.emplace(state_id, state);
    return {};
};

tl::expected<std::reference_wrapper<const StateEngine::State>, error::StateEngineError>
StateEngine::StateRegistry::get_state(const std::string& state_id) const {
    // check if a state with this ID exists
    auto iter = this->m_states.find(state_id);
    if (iter == this->m_states.end()) {
        return tl::unexpected(error::StateEngineError{
            error::StateEngineError::Type::State_GetError, "state with this ID does not exists" });
    }

    return iter->second;
};

tl::expected<void, error::StateEngineError>
StateEngine::State::load_transition(const std::string& transition_id, const State_can_transition_to& transition) {
    // check if a transition with the same ID already exists
    const auto query_result = this->get_transition(transition_id);
    if (query_result) {
        return tl::unexpected(error::StateEngineError{
            error::StateEngineError::Type::StateTransition_LoadError,
            "transition with this ID already exists" });
    }

    this->m_transitions.emplace(transition_id, transition);
    return {};
};

tl::expected<std::reference_wrapper<const StateEngine::State_can_transition_to>, error::StateEngineError>
StateEngine::State::get_transition(const std::string& transition_id) const {
    // check if a transition with this ID exists
    auto iter = this->m_transitions.find(transition_id);
    if (iter == this->m_transitions.end()) {
        return tl::unexpected(error::StateEngineError{
            error::StateEngineError::Type::StateTransition_GetError,
            "transition with this ID does not exists" });
    }

    return iter->second;
};

tl::expected<bool, error::StateEngineError>
StateEngine::State::can_transition_to(const std::string& transition_id) const {
    auto iter = this->m_transitions.find(transition_id);
    if (iter == this->m_transitions.end()) {
        return false;
    }
    return true;
};
