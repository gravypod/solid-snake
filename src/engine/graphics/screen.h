//
// Created by gravypod on 9/28/17.
//

#ifndef ENGINE_SCREEN_H
#define ENGINE_SCREEN_H

#define DEFAULT_SCREEN_WIDTH 400
#define DEFAULT_SCREEN_HEIGHT 400

static int screen_width = DEFAULT_SCREEN_WIDTH;
static int screen_height = DEFAULT_SCREEN_HEIGHT;

void screen_reshape(int width, int height);

static inline double screen_get_aspect_ratio() {
    return ((double) screen_width) / ((double) screen_height);
}

#endif //ENGINE_SCREEN_H
