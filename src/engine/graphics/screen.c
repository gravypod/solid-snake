//
// Created by gravypod on 9/28/17.
//

#include <lib/glad/glad.h>
#include "screen.h"

void screen_reshape(int width, int height)
{
    screen_width = width;
    screen_height = height;

    int smallest = screen_width < screen_height ? screen_width : screen_height;
    int largest  = screen_width > screen_height ? screen_width : screen_height;
    int diff     = (largest - smallest);

    if (screen_height == screen_width)
        glViewport(0, 0, screen_width, screen_height);
    else if (screen_width < screen_height)
        glViewport(0, diff/2, screen_width, screen_height - diff);
    else if (screen_height < screen_width)
        glViewport(diff/2, 0, screen_width - diff, screen_height);
}
