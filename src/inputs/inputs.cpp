//
// Created by bruno on 05/06/22.
//
#include "inputs.hpp"

static vec2 mouse_position_pixel_to_relative(vec2 const& p_pixel, int window_width, int window_height)
{
    float const x_rel = p_pixel.x / static_cast<float>(window_width);
    float const y_rel = p_pixel.y / static_cast<float>(window_height);

    return vec2{
            2.0f * x_rel - 1.0f,
            2.0f * (1.0f - y_rel) - 1.0f
    };
}



custom_inputs_keyboard_parameters::custom_inputs_keyboard_parameters()
        : shift(false), ctrl(false), up(false), down(false), right(false), left(false),
          w_key(false), a_key(false), d_key(false), s_key(false) {}

void custom_inputs_keyboard_parameters::update_from_glfw_key(int key, int action) {
    // Shift and CTRL
    if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action == GLFW_PRESS)
        shift = true;
    if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action == GLFW_RELEASE)
        shift = false;

    if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) && action == GLFW_PRESS)
        ctrl = true;
    if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) && action == GLFW_RELEASE)
        ctrl = false;

    // Up/Down/Left/Right
    if (key == GLFW_KEY_UP) {
        if (action == GLFW_PRESS) up = true;
        if (action == GLFW_RELEASE) up = false;
    }

    if (key == GLFW_KEY_DOWN) {
        if (action == GLFW_PRESS) down = true;
        if (action == GLFW_RELEASE) down = false;
    }

    if (key == GLFW_KEY_LEFT) {
        if (action == GLFW_PRESS) left = true;
        if (action == GLFW_RELEASE) left = false;
    }

    if (key == GLFW_KEY_RIGHT) {
        if (action == GLFW_PRESS) right = true;
        if (action == GLFW_RELEASE) right = false;
    }

    // AWSD keys

    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) a_key = true;
        if (action == GLFW_RELEASE) a_key = false;
    }
    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) s_key = true;
        if (action == GLFW_RELEASE) s_key = false;

    }
    if (key == GLFW_KEY_D) {

        if (action == GLFW_PRESS) d_key = true;
        if (action == GLFW_RELEASE) d_key = false;
    }
    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) w_key = true;
        if (action == GLFW_RELEASE) w_key = false;

    }
}

void custom_inputs_interaction_parameters::mouse_position_update(const vec2 &new_mouse_position) {
    mouse.position.previous = mouse.position.current;
    mouse.position.current = mouse_position_pixel_to_relative(new_mouse_position, window.width, window.height);
}
