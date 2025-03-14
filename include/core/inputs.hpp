#pragma once

enum class InputAction {
    move_forward,
    move_backwards,
    move_left,
    move_right,

    move_up,
    move_down,

    mouse_motion,

    left_mouse_click,
    right_mouse_click,

    left_mouse_hold,
    right_mouse_hold,

    toggle_debug,
    exit,
};