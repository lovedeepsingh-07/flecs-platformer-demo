#include "controller_engine.hpp"
#include <raylib.h>

void ControllerEngine::ControllerEngine::setup() {
    this->keymaps.emplace(0, ControllerKeymap{ .left = KEY_A, .right = KEY_D, .jump = KEY_W, .attack = KEY_E });
    this->keymaps.emplace(1, ControllerKeymap{ .left = KEY_H, .right = KEY_L, .jump = KEY_K, .attack = KEY_O });
};

const ControllerEngine::ControllerKeymap&
ControllerEngine::ControllerEngine::get_keymap(int keymap_id) const {
    return this->keymaps.at(keymap_id);
}
