#pragma once

#include <clay/clay.h>
#include <raylib.h>

namespace Utils {

Clay_Color RaylibColorToClayColor(Color rayColor);
void HandleClayErrors(Clay_ErrorData errorData);

}
