#include "src/scripting/interface.h"
#include "src/scripting/script.h"

#include "lib/glad/glad.h"
#include <GLFW/glfw3.h>

#include "drawing.h"

#define SCREEN_W 400
#define SCREEN_H 400

void update()
{
    static double start       = 0;
    const double current_time = glfwGetTime();
    const double delta        = current_time - start;
    start = current_time;

    s_update(delta);
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    {
        s_draw();
    }
    glFlush();
}


void init()
{
    if (!init_interface()) {
        printf("Failed to load scripting context\n");
        exit(1);
    }

    REGISTER_SCRIPT_INTERFACE("cube", s_cube, 5);

    // Load our script file
    if (!include_script("snake.js")) {
        printf("Error: %s\n", duk_safe_to_string(ctx, -1));
        exit(1);
    }

    s_init();
}


void on_screen_resize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void on_key_press(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    s_keypress((const unsigned char) key, action == GLFW_PRESS);
}


int main(int argc, char **argv) {

    init(); // Init our code.

    glfwInit();

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

    glfwSetFramebufferSizeCallback(window, on_screen_resize);
    glfwSetKeyCallback(window, on_key_press);

    // Screen is black
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1, 0.0, 1, -1.0, 1.0);

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
