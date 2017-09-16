#include "src/scripting/interface.h"
#include "src/scripting/script.h"

#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include "drawing.h"

#define ROWS 100
#define COLS 100

#define SCREEN_W 400
#define SCREEN_H 400


static long get_update_delta()
{
    static long start = -1;

    long delta;

    long current = glutGet(GLUT_ELAPSED_TIME);
    if (start == -1) {
        delta = 0;
    } else {
        delta = start - current;
    }
    start = current;
    return delta;
}


void draw()
{
    s_update(get_update_delta());
    glClear(GL_COLOR_BUFFER_BIT);
    {
        s_draw();
    }
    glFlush();
    glutPostRedisplay();
}


void init()
{
    if (!init_interface()) {
        printf("Failed to load scripting context\n");
        exit(1);
    }

    REGISTER_SCRIPT_INTERFACE("cube", s_cube, 3);

    // Load our script file
    if (!include_script("snake.js")) {
        printf("Error: %s\n", duk_safe_to_string(ctx, -1));
        exit(1);
    }

    s_init();
}


int main(int argc, char **argv) {

    init(); // Init our code.

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(SCREEN_W, SCREEN_H);
    glutCreateWindow("Hello World");

    // Screen is black
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, ROWS, 0.0, COLS, -1.0, 1.0);

    //glutIdleFunc(s_update);
    glutKeyboardFunc(s_keypress);
    glutDisplayFunc(draw);
    glutMainLoop();

    return 0;
}
