#pragma once

#include "clay/clay.h"
#include <string>
#include <vector>

// FIXME: just a temporary way of storing strings to last until the frame ends
namespace MemoryArena {
    class MemoryArena {
      public:
        std::vector<std::string> buffer;
        Clay_String get_string(const std::string& input) {
            this->buffer.emplace_back(input);
            const std::string& input_pointer = this->buffer.back();
            return Clay_String{
                .length = (int32_t)input_pointer.size(),
                .chars = input_pointer.c_str(),
            };
        }
    };
}
