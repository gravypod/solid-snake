#include <stdio.h>
#include <stdbool.h>
#include <memory.h>
#include "vulkan.h"
#include "wrappers.h"


#define NUM_WANTED_VALIDATION_LAYERS ((uint32_t) 2)
const char *wanted_validation_layers[NUM_WANTED_VALIDATION_LAYERS] = {
		"VK_LAYER_LUNARG_core_validation",
		"VK_LAYER_LUNARG_standard_validation"

};


static VKAPI_ATTR VkBool32 VKAPI_CALL on_error(
		VkDebugUtilsMessageSeverityFlagBitsEXT severity,
		VkDebugUtilsMessageTypeFlagsEXT type,
		const VkDebugUtilsMessengerCallbackDataEXT *data,
		void *user_date
)
{
	// Ignore lack of usage
	((void) severity);
	((void) type);
	((void) data);
	((void) user_date);

	printf("Validation Message: %s\n", data->pMessage);

	return VK_FALSE;
}


uint32_t vulkan_debug_num_validation_layers()
{
	return NUM_WANTED_VALIDATION_LAYERS;
}

const char **vulkan_debug_validation_layers()
{
	return wanted_validation_layers;
}


void vulkan_debug_init(vulkan *v)
{
	VkDebugUtilsMessengerCreateInfoEXT creation_request = {
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
							   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
							   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
						   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
						   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
			.pfnUserCallback = on_error,
			.pUserData = NULL // Optional
	};

	wrap_vulkan_create_debug_utils_messenger_ext(
			v->instance,
			&creation_request,
			NULL,
			&v->debugging.debug_utils_messenger_callback
	);
}

bool vulkan_debug_has_all_wanted_validation_layers(vulkan *v)
{

	for (uint32_t wanted_layer_index = 0; wanted_layer_index < NUM_WANTED_VALIDATION_LAYERS; wanted_layer_index++) {
		bool found = false;

		// Search array of available layers
		for (uint32_t i = 0; !found && i < v->layers.num_properties; i++) {
			found = strcmp(wanted_validation_layers[wanted_layer_index], v->layers.properties[i].layerName) == 0;
		}

		if (!found) {
			return false;
		}
	}

	return true;
}
