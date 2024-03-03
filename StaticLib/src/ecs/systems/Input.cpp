#include "Input.h"
#include "../../core/Defines.h"
#include <Windows.h>
#define PRINT(x) std::cout << x << std::endl

Input::Input() {}

Input::~Input() {}

void Input::update() {
    previousKeys = currentKeys;
    for (int i = 0; i < 256; ++i) {
        currentKeys[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }
    PRINT("Input udapted"); //Check si l'input est updated.
}

bool Input::isKeyDown(int keyCode) const {
    auto it = currentKeys.find(keyCode);
    return it == currentKeys.end() && it->second;
}

bool Input::isKeyUp(int keyCode) const {
    auto it = currentKeys.find(keyCode);
    return it == currentKeys.end() || !it->second;
}

bool Input::isKeyPressed(int keyCode) const {
    auto currentIt = currentKeys.find(keyCode);
    auto previousIt = previousKeys.find(keyCode);
    return currentIt != currentKeys.end() && currentIt->second &&
        (previousIt == previousKeys.end() || !previousIt->second);
}

bool Input::isMouseDown() const {
    return GetAsyncKeyState(VK_LBUTTON) & 0x8000; // Check si le click gauche est préssé 
}