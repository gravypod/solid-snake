
#include "lib/glad/glad.h"
#include <GLFW/glfw3.h>

#include "src/graphics/style.h"
#include "src/scripting/script.h"


#define SCREEN_W 400
#define SCREEN_H 400

void update()
{
    static double start       = 0;
    const double current_time = glfwGetTime();
    const double delta        = current_time - start;
    start = current_time;

    script.update(delta);
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    {
        script.draw();
    }
    glFlush();
}


void init()
{
    if (!init_interface()) {
        printf("Failed to load scripting context\n");
        exit(1);
    }

    // Load our script file
    if (!include_script("snake.js")) {
        printf("Error: %s\n", duk_safe_to_string(ctx, -1));
        exit(1);
    }
    script.init();
}


void on_screen_resize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void on_key_press(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    script.on_keypress((const unsigned char) key, action == GLFW_PRESS);
}


int main(int argc, char **argv) {



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

    glfwSetFramebufferSizeCallback(window, on_screen_resize);
    glfwSetKeyCallback(window, on_key_press);


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
