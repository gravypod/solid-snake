#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H


#include <stdbool.h>
#include "vulkan.h"

#define SCREEN_W 400
#define SCREEN_H 400

// this shouldn't be the way this is done
extern volatile float cursor_x;
extern volatile float cursor_y;


bool vulkan_window_init(vulkan *v);

bool vulkan_window_is_close_requested();

void vulkan_window_update();

void vulkan_window_cleanup();

int vulkan_window_width_get();

int vulkan_window_height_get();

#endif
