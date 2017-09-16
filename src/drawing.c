//
// Created by gravypod on 9/15/17.
//

#include "drawing.h"

#include <GL/gl.h>

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

