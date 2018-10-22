#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <lib/linmath/linmath.h>
#include <string.h>
#include "query.h"
#include "vulkan.h"
#include "config.h"
#include "debug.h"
#include "queues.h"
#include "window.h"
#include "surface.h"
#include "swapchain.h"
#include "pipeline.h"
#include "framebuffer.h"
#include "locking.h"


#include <src/engine/entity/entity.h>
#include <src/engine/entity/definitions/triangle.h>
#include <src/engine/entity/manager.h>

#include <src/engine/graphics/vulkan/commands/render.h>
#include <src/engine/graphics/vulkan/commands/buffer.h>

#include <src/engine/graphics/vulkan/shaders/shader.h>

#include <src/engine/graphics/vulkan/memory/memory.h>
#include <src/engine/graphics/vulkan/memory/vbuffer.h>

vulkan v = {
		.definition =  {
				.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
				.pApplicationName = "Solid Snake",
				.applicationVersion = VK_MAKE_VERSION(0, 0, 1),
				.pEngineName = "Solid Engine",
				.engineVersion = VK_MAKE_VERSION(0, 0, 1),
				.apiVersion = VK_API_VERSION_1_0
		},
		.devices.selected_device = VK_NULL_HANDLE
};

// TODO: This is terrible. Find a better way to do DI
vulkan *vulkan_pointer = &v;

cbuffer_pool_t *cpool;

VkResult vulkan_create_instance()
{
	VkInstanceCreateInfo creation_request = {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pApplicationInfo = &v.definition
	};

	creation_request.ppEnabledLayerNames = v.required_configuration.layers;
	creation_request.enabledLayerCount = v.required_configuration.num_layers;

	creation_request.ppEnabledExtensionNames = v.required_configuration.extensions;
	creation_request.enabledExtensionCount = v.required_configuration.num_extensions;

	return vkCreateInstance(&creation_request, NULL, &v.instance);
}

void vulkan_info_print()
{
	printf("Vulkan Initialized\n");

	printf("Loaded Vulkan Extensions: %u\n", v.extensions.num_properties);
	for (uint32_t i = 0; i < v.extensions.num_properties; i++) {
		printf("\t%d - %s (%d)\n", i, v.extensions.properties[i].extensionName, v.extensions.properties[i].specVersion);
	}

	printf("Loaded Vulkan Instance Layers: %u\n", v.layers.num_properties);
	for (uint32_t i = 0; i < v.layers.num_properties; i++) {
		printf("\t%d - %s (%d)\n", i, v.layers.properties[i].layerName, v.layers.properties[i].specVersion);
	}

	printf("Using %u as the main graphics queue\n", v.queues.main_rendering_queue_id);
}


bool vulkan_init()
{

	if (!vulkan_window_init(&v)) {
		printf("Failed to create window!\n");
		return false;
	}

	// Find hardware info
	if (!vulkan_hardware_query(&v)) {
		printf("Failed to query vulkan hardware!\n");
		return false;
	}

	// Choose extensions and layers to use
	vulkan_config_init(&v);

	// Create vulkan instance
	if (vulkan_create_instance() != VK_SUCCESS) {
		printf("Failed to create vulkan instance\n");
		return false;
	}

	// Start the debugger
	vulkan_debug_init(&v);

	// Select a device
	if ((v.devices.selected_device = vulkan_config_pick_physical_device(&v)) == VK_NULL_HANDLE) {
		printf("Failed to find suitable device!\n");
		return false;
	}

	// Make drawable surface
	if (!vulkan_surface_init(&v)) {
		printf("Failed to create drawing surface\n");
		return false;
	}

	// Find a queue with graphics pipeline support
	if (!vulkan_queues_init(&v)) {
		printf("Could not find graphics bit in chosen device!\n");
		return false;
	}

	if (!vulkan_swapchain_init(&v)) {
		return false;
	}

	if (!vulkan_shader_init()) {
		return false;
	}


	if (!vulkan_locking_init(&v)) {
		printf("Could not create locks\n");
		return false;
	}

	if (!vulkan_pipeline_init(&v)) {
		return false;
	}

	if (!vulkan_framebuffer_init(&v)) {
		return false;
	}

	if (!vulkan_memory_init(&v)) {
		printf("Failed to init memory\n");
		return false;
	}

	// TODO: We should NOT be passing the triangle position buffer in like this
	if (!(cpool = vulkan_commands_cpool_allocate(&v, v.swapchain.num_images * 2))) {
		printf("Failed to init command pool\n");
	}

	// Info to prove we have loaded everything
	vulkan_info_print();

	return true;
}

void vulkan_render()
{

	// Render starting configuration
	VkSemaphore waiting_semaphore = vulkan_locking_semphore_get_frame_buffer_image_available();
	VkPipelineStageFlags waiting_semaphore_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	// Render ending configuration
	VkSemaphore signaling_semaphore = vulkan_locking_semphore_get_rendering_finished();


	// Get the next image to render onto
	uint32_t swapchain_image_id = vulkan_swapchain_aquire(&v);

	// Get a free command buffer to use
	uint32_t cbuffer_id = vulkan_commands_cpool_cbuffer_id_take(cpool);

	vulkan_commands_buffer_begin(cpool, cbuffer_id);
	{
		vulkan_commands_render_begin(&v, cpool, cbuffer_id, swapchain_image_id);
		{
			VkCommandBuffer buffer = vulkan_commands_cpool_cbuffer_get(cpool, cbuffer_id);

			// Draw all entities into the command buffer
			entity_manager_draw(buffer);
		}
		vulkan_commands_render_end(cpool, cbuffer_id);
	}
	vulkan_commands_buffer_end(
			cpool, cbuffer_id,
			1, &waiting_semaphore, &waiting_semaphore_stage,
			1, &signaling_semaphore
	);


	// Schedule this render job to be presented
	VkPresentInfoKHR present_info = {
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			// Subscribe
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &signaling_semaphore,

			.swapchainCount = 1,
			.pSwapchains = &v.swapchain.swapchain,
			.pImageIndices = &swapchain_image_id,

			.pResults = NULL
	};

	vkQueuePresentKHR(v.queues.presenting, &present_info);
}

void vulkan_update()
{
	vulkan_window_update();
}

void vulkan_cleanup()
{
	vulkan_window_cleanup();
}
