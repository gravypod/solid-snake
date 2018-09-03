
#define GLFW_INCLUDE_VULKAN

#include "lib/glad/glad.h"
#include <GLFW/glfw3.h>
#include "src/engine/graphics/vulkan.h"
#include "src/engine/scripting/callbacks.h"
#include "src/engine/scripting/script.h"

#define SCREEN_W 400
#define SCREEN_H 400



void update() {
    static double start = 0;
    const double current_time = glfwGetTime();
    const double delta = current_time - start;
    start = current_time;

    game.update(delta);
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    {
        game.draw();
    }
    glFlush();
}


void init() {
    if (!init_interface()) {
        printf("Failed to load scripting context\n");
        exit(1);
    }

    // Load our game file
    if (!include_script("snake.js")) {
        printf("Error: %s\n", duk_safe_to_string(ctx, -1));
        exit(1);
    }
    game.init();
}


int main() {


    glfwInit();

    // Disable loading OpenGL and use GLFW for loading APIs
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Solid Engine", NULL, NULL);

    if (!window) {
        printf("Failed to create window!\n");
        glfwTerminate();
        return 1;
    }


    glfwMakeContextCurrent(window);


    if (!init_vulkan()) {
        printf("Failed to initialize graphics layer\n");
        return 1;
    }

    //init(); // Init our code.

    //REGISTER_GLFW_CALLBACKS();


    while (!glfwWindowShouldClose(window)) {
        //update();
        //draw();

        //glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanup_vulkan();

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
