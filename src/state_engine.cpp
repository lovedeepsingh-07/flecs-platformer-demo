#include "state_engine.hpp"

tl::expected<void, error::StateEngineError> StateEngine::StateEngine::setup() {
    sol::state lua_state;
    lua_state.open_libraries(sol::lib::base);
    lua_state.script_file("data/script.lua");

    auto registry_load_result =
        this->load_state_registry("cowboy", "data/cowboy.states.lua", lua_state);
    if (!registry_load_result) {
        return tl::unexpected(registry_load_result.error());
    }

    return {};
}

tl::expected<void, error::StateEngineError> StateEngine::StateEngine::load_state_registry(
    const std::string& registry_id, const std::string& registry_file_path, sol::state& lua_state
) {
    // check if a registry with the same ID already exists
    const auto query_result = this->get_state_registry(registry_id);
    if (query_result) {
        return tl::unexpected(error::StateEngineError{
            error::StateEngineError::Type::StateRegistry_LoadError,
            "registry with this ID already exists" });
    }


    lua_state.script_file(registry_file_path);
    sol::object STATES_object = lua_state["STATES"];
    if (!STATES_object.valid() || STATES_object.get_type() != sol::type::table) {
        return tl::unexpected(error::StateEngineError{
            error::StateEngineError::Type::StateRegistry_LoadError,
            "unable to read 'STATES' table" });
    }
    sol::table STATES_table = STATES_object.as<sol::table>();

    StateRegistry state_registry;

    for (auto&& curr_state_kv : STATES_table) {
        State curr_state;

        // id
        curr_state.id = curr_state_kv.first.as<std::string>();

        sol::object curr_state_object = curr_state_kv.second;
        if (!curr_state_object.valid() || curr_state_object.get_type() != sol::type::table) {
            return tl::unexpected(error::StateEngineError{
                error::StateEngineError::Type::StateRegistry_LoadError,
                "failed to read state object" });
        }
        sol::table curr_state_table = curr_state_object.as<sol::table>();

        // damage
        sol::object damage_object = curr_state_table["damage"];
        if (damage_object.valid() && damage_object.get_type() == sol::type::number) {
            curr_state.offensive = true;
            curr_state.damage = damage_object.as<int>();
        }

        // hitbox
        sol::object hitbox_object = curr_state_table["hitbox"];
        if (hitbox_object.valid() && hitbox_object.get_type() == sol::type::table) {
            sol::table hitbox_table = hitbox_object.as<sol::table>();
            curr_state.hitbox = Rectangle{
                hitbox_table[1],
                hitbox_table[2],
                hitbox_table[3],
                hitbox_table[4],
            };
        } else {
            curr_state.offensive = false;
        }

        // can_transition_to
        sol::object transition_object = curr_state_table["can_transition_to"];
        if (!transition_object.valid() || transition_object.get_type() != sol::type::table) {
            return tl::unexpected(error::StateEngineError{
                error::StateEngineError::Type::StateRegistry_LoadError,
                "failed to read 'can_transition_to' object" });
        }
        sol::table transition_table = transition_object.as<sol::table>();
        for (auto&& curr_transition_kv : transition_table) {
            State_can_transition_to curr_transition;
            curr_transition.id = curr_transition_kv.first.as<std::string>();
            curr_transition.frame = curr_transition_kv.second.as<int>();
            const auto transition_load_result =
                curr_state.load_transition(curr_transition.id, curr_transition);
            if (!transition_load_result) {
                return tl::unexpected(error::StateEngineError{
                    error::StateEngineError::Type::StateRegistry_LoadError,
                    transition_load_result.error().message });
            }
        }

        // animation_data
        sol::object animation_object = curr_state_table["animation_data"];
        if (!animation_object.valid() || animation_object.get_type() != sol::type::table) {
            return tl::unexpected(error::StateEngineError{
                error::StateEngineError::Type::StateRegistry_LoadError,
                "failed to read 'animation_data' object" });
        }
        sol::table animation_table = animation_object.as<sol::table>();
        State_animation_data animation_data;
        animation_data.texture_id = animation_table["texture_id"];
        animation_data.loop = animation_table["loop"];
        sol::object frames_object = animation_table["frames"];
        if (!frames_object.valid() || frames_object.get_type() != sol::type::table) {
            return tl::unexpected(error::StateEngineError{
                error::StateEngineError::Type::StateRegistry_LoadError,
                "failed to read 'frames' object" });
        }
        sol::table frames_table = frames_object.as<sol::table>();
        for (auto&& curr_frame_kv : frames_table) {
            sol::table curr_frame_table = curr_frame_kv.second.as<sol::table>();
            animation_data.frames.emplace_back(AnimationFrame{
                ._type = curr_frame_table[1],
                .source_rect = Rectangle{
                    curr_frame_table[2][1],
                    curr_frame_table[2][2],
                    curr_frame_table[2][3],
                    curr_frame_table[2][4],
                } });
        }
        curr_state.animation_data = animation_data;
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

bool StateEngine::State::can_transition_to(const std::string& transition_id) const {
    auto iter = this->m_transitions.find(transition_id);
    return iter != this->m_transitions.end();
};
