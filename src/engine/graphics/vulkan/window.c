#include "window.h"

#include <stdbool.h>
#include <stdio.h>

GLFWwindow *window;

bool vulkan_window_init(vulkan *v) {
    glfwInit();

    // Disable loading OpenGL and use GLFW for loading APIs
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = v->g.window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Solid Engine", NULL, NULL);

    if (!v->g.window) {
        printf("Failed to create window!\n");
        glfwTerminate();
        return false;
    }


    glfwMakeContextCurrent(v->g.window);

    return true;
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
