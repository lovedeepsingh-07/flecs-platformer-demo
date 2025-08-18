#pragma once

#include <clay/clay.h>
#include <raylib.h>

namespace game_utils {

Clay_Color RaylibColorToClayColor(Color rayColor);
void HandleClayErrors(Clay_ErrorData errorData);

}
