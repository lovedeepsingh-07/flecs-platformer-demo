#pragma once

#include <cstdint>
#include <string>

namespace error {
    struct StateEngineError {
        enum class Type : std::uint8_t {
            StateRegistry_LoadError,
            StateRegistry_GetError,
            State_LoadError,
            State_GetError,
            StateTransition_LoadError,
            StateTransition_GetError,
        } type;
        std::string message;
        StateEngineError(Type t, std::string msg)
        : type(t), message(std::move(msg)) {}
    };
}
