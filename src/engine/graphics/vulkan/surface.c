#include "surface.h"

bool vulkan_surface_init(vulkan *v)
{
	return glfwCreateWindowSurface(v->instance, v->g.window, NULL, &v->surface) == VK_SUCCESS;
}
