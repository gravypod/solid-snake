//
// Created by gravypod on 9/15/17.
//

#include "drawing.h"

#include <GL/gl.h>

// Draw a cube on the screen
void cube(float x, float y, float width, float height, int food)
{

    // Food is red, snake is white
    if (food)
        glColor3f(1.0, 0.0, 0.0);
    else
        glColor3f(1.0, 1.0, 1.0);


    glBegin(GL_POLYGON);
    {
        glVertex3f(        x,          y, 0.0);
        glVertex3f(x + width,          y, 0.0);
        glVertex3f(x + width, y + height, 0.0);
        glVertex3f(        x, y + height, 0.0);
    }
    glEnd();
}

