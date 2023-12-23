#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <unordered_map>

constexpr int MOUSE_LEFT = 1;
constexpr int MOUSE_RIGHT = 1;

class InputManager
{
public:
    InputManager();
    ~InputManager();

    void set_key_down(int key);
    void set_key_up(int key);

    bool is_key_pressed(int key);

    void set_mouse_position(int x, int y);
    void set_mouse_button_down(int button);
    void set_mouse_button_up(int button);

    std::pair<int, int> get_mouse_position();

    bool is_mouse_button_pressed(int button);


private:
    std::unordered_map<int, bool> key_state;
    struct MouseState
    {
        int x, y;
        bool left;
        bool right;
    } mouse_state;
};

#endif
