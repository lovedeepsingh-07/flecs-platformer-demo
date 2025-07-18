#include "utils.hpp"
#include "clay/clay.h"
#include "raylib.h"
#include <iostream>

auto Utils::RaylibColorToClayColor(Color rayColor) -> Clay_Color {
    Clay_Color clayColor;
    clayColor.r = rayColor.r;
    clayColor.g = rayColor.g;
    clayColor.b = rayColor.b;
    clayColor.a = rayColor.a;
    return clayColor;
}

void Utils::HandleClayErrors(Clay_ErrorData errorData) {
    std::cout << errorData.errorText.chars << "\n";
}
