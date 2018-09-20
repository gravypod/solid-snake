#include "window.h"

#include <stdbool.h>
#include <stdio.h>

GLFWwindow *window;

volatile float cursor_x;
volatile float cursor_y;

void vulkan_on_mouse_move(GLFWwindow *window, double x, double y)
{
    (void) window;
    cursor_x = (float) x;
    cursor_y = (float) y;
}


bool vulkan_window_init(vulkan *v) {
    glfwInit();

    // Disable loading OpenGL and use GLFW for loading APIs
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = v->g.window = glfwCreateWindow(
            vulkan_window_width_get(), vulkan_window_height_get(),
            "Solid Engine",
            NULL,
            NULL
    );

    if (!v->g.window) {
        printf("Failed to create window!\n");
        glfwTerminate();
        return false;
    }

    // TODO: Move into it's own location
    glfwSetCursorPosCallback(v->g.window, vulkan_on_mouse_move);

    glfwMakeContextCurrent(v->g.window);

    return true;
}

int vulkan_window_width_get() {
    return SCREEN_W;
}

int vulkan_window_height_get() {
    return SCREEN_H;
}

void vulkan_window_update() {
    glfwPollEvents();
}

void vulkan_window_cleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool vulkan_window_is_close_requested() {
    if (!window) {
        return true;
    }
    return (bool) glfwWindowShouldClose(window);
}
