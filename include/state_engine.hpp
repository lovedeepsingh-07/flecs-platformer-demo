#pragma once

#include "error.hpp"
#include <functional>
#include <raylib.h>
#include <tl/expected.hpp>
#include <yaml-cpp/yaml.h>

namespace StateEngine {

struct AnimationFrame {
    std::string _type;
    Rectangle source_rect;
};

struct State_animation_data {
    std::string texture_id;
    bool loop;
    std::vector<AnimationFrame> frames;
};

struct State_can_transition_to {
    std::string id;
    int frame;
};

class State {
  private:
    std::unordered_map<std::string, State_can_transition_to> m_transitions;

  public:
    std::string id;
    State_animation_data animation_data;
    bool offensive;
    Rectangle hitbox;

    tl::expected<void, error::StateEngineError>
    load_transition(const std::string& transition_id, const State_can_transition_to& transition);
    tl::expected<std::reference_wrapper<const State_can_transition_to>, error::StateEngineError>
    get_transition(const std::string& transition_id) const;
    bool can_transition_to(const std::string& transition_id) const;
};

class StateRegistry {
  private:
    std::unordered_map<std::string, State> m_states;

  public:
    tl::expected<void, error::StateEngineError>
    load_state(const std::string& state_id, const State& state);
    tl::expected<std::reference_wrapper<const State>, error::StateEngineError>
    get_state(const std::string& state_id) const;
};

class StateEngine {
  private:
    std::unordered_map<std::string, StateRegistry> m_state_registries;

  public:
    tl::expected<void, error::StateEngineError> setup();
    tl::expected<void, error::StateEngineError>
    load_state_registry(const std::string& registry_id, const std::string& registry_file_path);
    tl::expected<std::reference_wrapper<const StateRegistry>, error::StateEngineError>
    get_state_registry(const std::string& registry_id) const;
};

}
