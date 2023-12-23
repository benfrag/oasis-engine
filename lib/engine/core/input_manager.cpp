#include "engine/core/input_manager.h"

InputManager::InputManager()
{
    mouse_state = {0, 0, false, false};
}

InputManager::~InputManager() {}

void InputManager::set_key_down(int key)
{
    key_state[key] = true;
}

void InputManager::set_key_up(int key)
{
    key_state[key] = false;
}

bool InputManager::is_key_pressed(int key)
{
    auto it = key_state.find(key);
    if (it != key_state.end())
        return it->second;
    return false;
}

void InputManager::set_mouse_position(int x, int y)
{
    mouse_state.x = x;
    mouse_state.y = y;
}

void InputManager::set_mouse_button_down(int button)
{
    if (button == MOUSE_LEFT)
        mouse_state.left = true;
    if (button == MOUSE_RIGHT)
        mouse_state.right = true;
}

void InputManager::set_mouse_button_up(int button)
{
    if (button == MOUSE_LEFT)
        mouse_state.left = false;
    if (button == MOUSE_RIGHT)
        mouse_state.right = false;
}

std::pair<int, int> InputManager::get_mouse_position()
{
    return {mouse_state.x, mouse_state.y};
}

bool InputManager::is_mouse_button_pressed(int button)
{
    if (button == MOUSE_LEFT)
        return mouse_state.left;
    if (button == MOUSE_RIGHT)
        return mouse_state.right;

    return false;
}
