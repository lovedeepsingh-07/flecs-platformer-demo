#include <unordered_map>

namespace ControllerEngine {
    struct ControllerKeymap {
        int left;
        int right;
        int jump;
        int attack;
    };
    class ControllerEngine {
      public:
        std::unordered_map<int, ControllerKeymap> keymaps;

        ControllerEngine() = default;
        ~ControllerEngine() = default;

        void setup();
        const ControllerKeymap& get_keymap(int keymap_id) const;
    };
};
