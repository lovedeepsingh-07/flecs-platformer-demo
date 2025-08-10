#pragma once

#include <clay/clay.h>
#include <raylib.h>
#include <string>

namespace Utils {

struct ShapeUserData {
    std::string _id;
    void* _data;
};
struct RayCastUserData {
    std::string _id;
    void* _data;
};

Clay_Color RaylibColorToClayColor(Color rayColor);
void HandleClayErrors(Clay_ErrorData errorData);

}
