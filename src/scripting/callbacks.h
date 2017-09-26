//
// Created by gravypod on 9/26/17.
//

#ifndef ENGINE_CALLBACKS_H
#define ENGINE_CALLBACKS_H

#include <GLFW/glfw3.h>

void on_screen_resize(GLFWwindow *window, int width, int height);
void on_key_press(GLFWwindow* window, int key, int scancode, int action, int mods);
void on_mouse_click(GLFWwindow *window, int button, int action, int mods);
void on_mouse_scroll(GLFWwindow *window, double x, double y);
void on_mouse_move(GLFWwindow *window, double x, double y);

/**
 * This macro is called when it is time to register all of our
 * glfw callbacks. If you add, or change, a callback please
 * update this.
 */
#define REGISTER_GLFW_CALLBACKS() { \
    glfwSetFramebufferSizeCallback(window, on_screen_resize); \
    glfwSetKeyCallback(window, on_key_press); \
    glfwSetScrollCallback(window, on_mouse_scroll); \
    glfwSetMouseButtonCallback(window, on_mouse_click); \
    glfwSetCursorPosCallback(window, on_mouse_move); \
}

#endif //ENGINE_CALLBACKS_H
