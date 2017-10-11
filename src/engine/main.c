
#include "lib/glad/glad.h"
#include <GLFW/glfw3.h>
#include "src/engine/scripting/callbacks.h"
#include "src/engine/scripting/script.h"

#define SCREEN_W 400
#define SCREEN_H 400

void update()
{
    static double start       = 0;
    const double current_time = glfwGetTime();
    const double delta        = current_time - start;
    start = current_time;

    game.update(delta);
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    {
        game.draw();
    }
    glFlush();
}


void init()
{
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Solid Engine", NULL, NULL);

    if (!window)
    {
        printf("Failed to create window!\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf("Failed to load open GL api\n");
        return 1;
    }


    // Screen is black
    glClearColor(0.0, 0.0, 0.0, 0.0);

    init(); // Init our code.

    REGISTER_GLFW_CALLBACKS();

    while (!glfwWindowShouldClose(window))
    {
        update();
        draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}
