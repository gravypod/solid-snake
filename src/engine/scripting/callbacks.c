//
// Created by gravypod on 9/26/17.
//

#include "callbacks.h"
#include "script.h"
#include "src/engine/graphics/screen.h"

void on_screen_resize(GLFWwindow *window, int width, int height)
{
    (void) window;
    screen_reshape(width, height);
}


void on_key_press(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void) window;
    (void) scancode;
    (void) mods;
    game.on_keypress((const unsigned char) key, action == GLFW_PRESS);
}

void on_mouse_click(GLFWwindow *window, int button, int action, int mods)
{
    (void) window;
    (void) mods;
    game.on_mouse_click(button, action == GLFW_PRESS);
}

void on_mouse_scroll(GLFWwindow *window, double x, double y)
{
    (void) window;
    game.on_mouse_scroll(x, y);
}

void on_mouse_move(GLFWwindow *window, double x, double y)
{
    (void) window;
    game.on_mouse_move(x, y);
}
