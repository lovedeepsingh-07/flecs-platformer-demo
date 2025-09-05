#include "controller_engine.hpp"
#include <raylib.h>

tl::expected<void, error::ControllerEngineError> ControllerEngine::ControllerEngine::setup() {
    sol::state lua_state;
    lua_state.open_libraries(sol::lib::base);
    lua_state.script_file("data/keys.lua");
    lua_state.script_file("data/config.lua");

    sol::object KEYS_object = lua_state["KEYS"];
    if (!KEYS_object.valid() || KEYS_object.get_type() != sol::type::table) {
        return tl::unexpected(error::ControllerEngineError{
            error::ControllerEngineError::Type::LoadError, "unable to read 'KEYS' table" });
    }
    sol::table KEYS_table = KEYS_object.as<sol::table>();

    sol::object KEYMAPS_object = lua_state["KEYMAPS"];
    if (!KEYMAPS_object.valid() || KEYMAPS_object.get_type() != sol::type::table) {
        return tl::unexpected(error::ControllerEngineError{
            error::ControllerEngineError::Type::LoadError, "unable to read 'KEYMAPS' table" });
    }
    sol::table KEYMAPS_table = KEYMAPS_object.as<sol::table>();

    for (auto&& keymap_kv : KEYMAPS_table) {
        sol::object keymap_object = keymap_kv.second;
        if (!keymap_object.valid() || keymap_object.get_type() != sol::type::table) {
            return tl::unexpected(error::ControllerEngineError{
                error::ControllerEngineError::Type::LoadError, "unable to read keymap table" });
        }
        sol::table keymap_table = keymap_object.as<sol::table>();


        ControllerKeymap curr_keymap{
            .left = KEYS_table[keymap_table["left"]],
            .right = KEYS_table[keymap_table["right"]],
            .down = KEYS_table[keymap_table["down"]],
            .jump = KEYS_table[keymap_table["jump"]],
            .light_attack = KEYS_table[keymap_table["light_attack"]],
            .heavy_attack = KEYS_table[keymap_table["heavy_attack"]],
        };

        int keymap_id = 0;
        std::string keymap_id_string = keymap_kv.first.as<std::string>();
        if (keymap_id_string == "player") {
            keymap_id = 0;
        } else if (keymap_id_string == "enemy") {
            keymap_id = 1;
        }

        this->keymaps.emplace(keymap_id, curr_keymap);
    }

    return {};
};

const ControllerEngine::ControllerKeymap&
ControllerEngine::ControllerEngine::get_keymap(int keymap_id) const {
    return this->keymaps.at(keymap_id);
}
