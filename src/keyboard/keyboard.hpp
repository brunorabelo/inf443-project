//
// Created by bruno on 05/06/22.
//

#ifndef PROJECT_KEYBOARD_HPP
#define PROJECT_KEYBOARD_HPP

#include "cgp/cgp.hpp"

using namespace cgp;

class keyboard {

};

struct custom_inputs_keyboard_parameters {

    bool shift;
    bool ctrl;
    bool up, down, right, left, w_key, s_key, a_key, d_key;

    custom_inputs_keyboard_parameters();

    void update_from_glfw_key(int key, int action);
};

struct custom_inputs_interaction_parameters {
    windows_dimension_structure window;
    custom_inputs_keyboard_parameters keyboard;
    inputs_mouse_parameters mouse;

    void mouse_position_update(vec2 const &new_mouse_position);
};

#endif //PROJECT_KEYBOARD_HPP
