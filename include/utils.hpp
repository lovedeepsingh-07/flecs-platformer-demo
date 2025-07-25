#pragma once

#include "clay/clay.h"
#include "raylib.h"

namespace Utils {

auto RaylibColorToClayColor(Color rayColor) -> Clay_Color;
void HandleClayErrors(Clay_ErrorData errorData);

}
