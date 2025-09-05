#include "error.hpp"
#include <sol/sol.hpp>
#include <tl/expected.hpp>
#include <unordered_map>

namespace ControllerEngine {
    struct ControllerKeymap {
        int left;
        int right;
        int down;
        int jump;
        int light_attack;
        int heavy_attack;
    };
    class ControllerEngine {
      public:
        std::unordered_map<int, ControllerKeymap> keymaps;

        ControllerEngine() = default;
        ~ControllerEngine() = default;

        tl::expected<void, error::ControllerEngineError> setup();
        const ControllerKeymap& get_keymap(int keymap_id) const;
    };
};
