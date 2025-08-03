#include <raylib.h>
#include <yaml-cpp/yaml.h>

namespace StateEngine {

struct State_can_transition_to {
    std::string id;
    int frame;
};

struct State_animation_data {
    std::string texture_id;
    std::vector<Rectangle> frames;
};

struct State {
    std::string id;
    std::unordered_map<std::string, State_can_transition_to> can_transition_to;
    State_animation_data animation_data;
};

using StateRegistry = std::unordered_map<std::string, State>;

class StateEngine {
  private:
    std::unordered_map<std::string, StateRegistry> m_state_registries;

  public:
    void load_state_registry(const std::string& registry_id, const std::string& registry_file_path);
    StateRegistry& get_state_registry(const std::string& registry_id);
};

}
