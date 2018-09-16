

#include "lib/glad/glad.h"
#include <GLFW/glfw3.h>
#include <src/engine/graphics/vulkan/vulkan.h>
#include <src/engine/graphics/vulkan/window.h>
#include <src/engine/util/dict.h>
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
    if (!vulkan_init()) {
        printf("Failed to initialize graphics layer\n");
        return 1;
    }

    //init(); // Init our code.

    //REGISTER_GLFW_CALLBACKS();


    while (!vulkan_window_is_close_requested()) {
        //update();
        //draw();

        //glfwSwapBuffers(window);
        vulkan_update();
        vulkan_render();
    }

    vulkan_cleanup();

    return 0;
}
