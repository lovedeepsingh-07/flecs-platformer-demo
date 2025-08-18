#include "utils.hpp"
#include <iostream>

Clay_Color game_utils::RaylibColorToClayColor(Color rayColor) {
    Clay_Color clayColor;
    clayColor.r = rayColor.r;
    clayColor.g = rayColor.g;
    clayColor.b = rayColor.b;
    clayColor.a = rayColor.a;
    return clayColor;
}

void game_utils::HandleClayErrors(Clay_ErrorData errorData) {
    std::cout << errorData.errorText.chars << "\n";
}
