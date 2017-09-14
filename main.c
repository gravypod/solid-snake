#include "duktape.h"

#include <GLFW/glfw3.h>
#include <GL/glut.h>


#include <sys/stat.h>

#define ROWS 100
#define COLS 100

#define SCREEN_W 800
#define SCREEN_H 800

static duk_ret_t native_print(duk_context *ctx) {
    duk_push_string(ctx, " ");
    duk_insert(ctx, 0);
    duk_join(ctx, duk_get_top(ctx) - 1);
    printf("%s\n", duk_safe_to_string(ctx, -1));
    return 0;
}


duk_context *ctx;

off_t fsize(const char *filename)
{
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}


// Load our logic code.
void load_file(duk_context *ctx, const char *filename)
{
    const size_t file_size = (size_t) fsize(filename);
    FILE *f;

    if (file_size == -1 || !(f = fopen(filename, "rb"))) {
        duk_push_undefined(ctx);
        return;
    }

    size_t data_left = file_size;
    char *buffer = malloc(file_size + 1);
    char *tmp    = buffer;

    while (data_left > 0)
    {
        const size_t len = fread((void *) tmp, sizeof(char), sizeof(buffer), f);
        data_left -= len;
        tmp       += len;
    }
    buffer[file_size] = 0;

    fclose(f);

    duk_push_lstring(ctx, (const char *) buffer, (duk_size_t) file_size);
}

// Draw a cube on the screen
void cube(float x, float y, int food)
{

#define CUBE_WIDTH  1
#define CUBE_HEIGHT 1

    // Food is red, snake is white
    if (food)
        glColor3f(1.0, 0.0, 0.0);
    else
        glColor3f(1.0, 1.0, 1.0);


    glBegin(GL_POLYGON);
    {
        glVertex3f(x,               y,              0.0);
        glVertex3f(x + CUBE_HEIGHT, y,              0.0);
        glVertex3f(x + CUBE_HEIGHT, y + CUBE_WIDTH, 0.0);
        glVertex3f(x              , y + CUBE_WIDTH, 0.0);
    }
    glEnd();
}


// Interface layer between javascript and C cube
duk_ret_t s_cube(duk_context *ctx)
{
    float    x = (float) duk_require_number(ctx, 0);
    float    y = (float) duk_require_number(ctx, 1);
    int   food = (int)  duk_require_boolean(ctx, 2);
    //printf("Cube called\n");
    cube(x, y, food);
    return 1;
}

// Call script game.render();
void s_draw()
{
    //printf("Drawing\n");
    //duk_get_global_string(ctx, "game");
    //duk_push_string(ctx, "render");
    //duk_pcall_prop(ctx, -2, 1);
    //duk_pop_2(ctx);

    duk_get_global_string(ctx, "game");
    duk_push_string(ctx, "render");
    duk_pcall_prop(ctx, -2, 0 /*nargs*/);

    //printf("\tFinished\n");
}

void s_init()
{
    duk_get_global_string(ctx, "game");
    duk_push_string(ctx, "init");
    duk_pcall_prop(ctx, -2, 0 /*nargs*/);
}


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

void init()
{
    ctx = duk_create_heap_default();

    if (!ctx) {
        printf("Failed to load scripting context\n");
        exit(1);
    }


    // Add our native cube function.
    //duk_push_global_object(ctx);

/*
    duk_push_global_object(ctx);
    duk_put_global_string(ctx, "game");*/

    duk_push_c_function(ctx, native_print, DUK_VARARGS);
    duk_put_global_string(ctx, "print");

    duk_push_c_function(ctx, s_cube, 3 /*nargs*/);
    duk_put_global_string(ctx, "cube");

    // Load our script file
    load_file(ctx, "snake.js");
    if (duk_peval(ctx) != 0) {
        printf("Error: %s\n", duk_safe_to_string(ctx, -1));
        exit(1);
    }
    //duk_pop(ctx);


    //duk_eval_file_noresult(ctx, "snake.js");

    s_init();
}


// Call script game.update()
void s_update()
{
    long delta = get_update_delta();
    //printf("Updating (delta: %li)\n", delta);
    /*duk_get_global_string(ctx, "game");
    duk_push_string(ctx, "update");
    duk_push_number(ctx, delta);
    duk_pcall_prop(ctx, -3, 1);
    duk_pop(ctx);*/

    duk_get_global_string(ctx, "game");
    duk_push_string(ctx, "update");
    duk_push_number(ctx, delta);
    duk_pcall_prop(ctx, -3, 1 /*nargs*/);

    //printf("\nFinished\n");
    glutPostRedisplay();
}

void s_keypress(unsigned char key, int x, int y)
{

    duk_get_global_string(ctx, "game");
    duk_push_string(ctx, "keypress");
    duk_push_number(ctx, key);
    duk_pcall_prop(ctx, -3, 1 /*nargs*/);
}


void draw()
{
    s_update();
    glClear(GL_COLOR_BUFFER_BIT);
    {
        s_draw();
    }
    glFlush();
    glutPostRedisplay();
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
