#include <malloc.h>
#include "swapchain.h"
#include "window.h"
#include "locking.h"

VkSurfaceFormatKHR vulkan_swapchain_select_format(vulkan *v)
{
	VkSurfaceFormatKHR best_format = {
			.format = VK_FORMAT_B8G8R8A8_UNORM,
			.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
	};

	if (v->swapchain.formats[0].format == VK_FORMAT_UNDEFINED) {
		return best_format;
	} else {
		for (uint32_t i = 0; i < v->swapchain.num_formats; i++) {
			VkSurfaceFormatKHR found_format = v->swapchain.formats[i];
			if (found_format.format == best_format.format && found_format.colorSpace == best_format.colorSpace) {
				return found_format;
			}
		}
	}

	return v->swapchain.formats[0];
}

VkPresentModeKHR vulkan_swapchain_select_presentation_mode(vulkan *v)
{
	VkPresentModeKHR mode_order[] = {
			VK_PRESENT_MODE_MAILBOX_KHR,
			VK_PRESENT_MODE_IMMEDIATE_KHR,
			VK_PRESENT_MODE_FIFO_KHR
	};

	for (uint32_t mode_order_idx = 0; sizeof(mode_order) / sizeof(VkPresentModeKHR); mode_order_idx++) {
		for (uint32_t i = 0; i < v->swapchain.num_modes; i++) {
			if (v->swapchain.modes[i] == mode_order[mode_order_idx]) {
				return v->swapchain.modes[i];
			}
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D vulkan_swapchain_choose_extent(vulkan *v)
{
	if (v->swapchain.capabilities.currentExtent.width != UINT32_MAX) {
		return v->swapchain.capabilities.currentExtent;
	}

	VkExtent2D minimum = v->swapchain.capabilities.minImageExtent;
	VkExtent2D maximum = v->swapchain.capabilities.maxImageExtent;

	VkExtent2D extent = {
			SCREEN_W, SCREEN_H
	};


	// Clamp width and height between minimum and maximum
	if (minimum.width > extent.width) {
		extent.width = minimum.width;
	}
	if (maximum.width < extent.width) {
		extent.width = maximum.width;
	}


	if (minimum.height > extent.height) {
		extent.height = minimum.height;
	}
	if (maximum.height < extent.height) {
		extent.height = maximum.height;
	}

	return extent;
}

uint32_t vulkan_swapchain_choose_image_count(vulkan *v)
{
	uint32_t count = v->swapchain.capabilities.minImageCount + 1;
	if (v->swapchain.capabilities.maxImageCount > 0 && v->swapchain.capabilities.maxImageCount < count) {
		count = v->swapchain.capabilities.maxImageCount;
	}
	return count;
}

VkResult vulkan_swapchain_create(vulkan *v)
{
	v->swapchain.format = vulkan_swapchain_select_format(v);
	v->swapchain.extent = vulkan_swapchain_choose_extent(v);

	VkSwapchainCreateInfoKHR request = {
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.surface = v->surface,
			.minImageCount = vulkan_swapchain_choose_image_count(v),
			.imageFormat = v->swapchain.format.format,
			.imageColorSpace = v->swapchain.format.colorSpace,
			.imageExtent = v->swapchain.extent,
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,

			.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices = NULL,

			.preTransform = v->swapchain.capabilities.currentTransform,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,

			.presentMode = vulkan_swapchain_select_presentation_mode(v),
			.clipped = VK_TRUE,

			.oldSwapchain = VK_NULL_HANDLE
	};

	if (v->queues.main_presentation_queue_id != v->queues.main_rendering_queue_id) {
		request.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		request.queueFamilyIndexCount = v->queues.num_queue_families;
		request.pQueueFamilyIndices = v->queues.queue_families;
	}

	return vkCreateSwapchainKHR(v->devices.logical_device, &request, NULL, &v->swapchain.swapchain);
}

VkResult vulkan_swapchan_get_images(vulkan *v)
{

	vkGetSwapchainImagesKHR(v->devices.logical_device, v->swapchain.swapchain, &v->swapchain.num_images, NULL);
	v->swapchain.images = malloc(sizeof(VkImage) * v->swapchain.num_images);
	return vkGetSwapchainImagesKHR(v->devices.logical_device, v->swapchain.swapchain, &v->swapchain.num_images,
								   v->swapchain.images);
}

bool vulkan_swapchain_make_image_views(vulkan *v)
{
	v->swapchain.image_views = malloc(sizeof(VkImageView) * v->swapchain.num_images);
	for (uint32_t i = 0; i < v->swapchain.num_images; i++) {
		VkImageViewCreateInfo request = {
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.image = v->swapchain.images[i],
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = v->swapchain.format.format,
				.components = {
						.r = VK_COMPONENT_SWIZZLE_IDENTITY,
						.g = VK_COMPONENT_SWIZZLE_IDENTITY,
						.b = VK_COMPONENT_SWIZZLE_IDENTITY,
						.a = VK_COMPONENT_SWIZZLE_IDENTITY,
				},
				.subresourceRange = {
						.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
						.baseMipLevel = 0,
						.levelCount = 1,
						.baseArrayLayer = 0,
						.layerCount = 1
				}
		};

		if (vkCreateImageView(v->devices.logical_device, &request, NULL, &v->swapchain.image_views[i]) != VK_SUCCESS) {
			return false;
		}
	}
	return true;
}

bool vulkan_swapchain_init(vulkan *v)
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(v->devices.selected_device, v->surface, &v->swapchain.capabilities);
	vkGetPhysicalDeviceSurfaceFormatsKHR(v->devices.selected_device, v->surface, &v->swapchain.num_formats, NULL);
	vkGetPhysicalDeviceSurfacePresentModesKHR(v->devices.selected_device, v->surface, &v->swapchain.num_modes, NULL);

	v->swapchain.formats = malloc(sizeof(VkSurfaceFormatKHR) * v->swapchain.num_formats);
	v->swapchain.modes = malloc(sizeof(VkPresentModeKHR) * v->swapchain.num_modes);

	vkGetPhysicalDeviceSurfaceFormatsKHR(v->devices.selected_device, v->surface, &v->swapchain.num_formats,
										 v->swapchain.formats);
	vkGetPhysicalDeviceSurfacePresentModesKHR(v->devices.selected_device, v->surface, &v->swapchain.num_modes,
											  v->swapchain.modes);


	if (!v->swapchain.num_modes || !v->swapchain.num_formats) {
		printf("Could not find a supported swapchain mode or format\n");
		return false;
	}

	if (vulkan_swapchain_create(v) != VK_SUCCESS) {
		printf("Failed to create swapchain instance from configuration\n");
		return false;
	}

	if (vulkan_swapchan_get_images(v) != VK_SUCCESS) {
		printf("Failed to load images out of swapchain\n");
		return false;
	}

	if (!vulkan_swapchain_make_image_views(v)) {
		printf("Failed to make image views\n");
		return false;
	}

	return true;
}

uint32_t vulkan_swapchain_aquire(vulkan *v)
{
	uint32_t image_index;
	
	vkAcquireNextImageKHR(
			v->devices.logical_device,
			v->swapchain.swapchain,
			UINT64_MAX,
			vulkan_locking_semphore_get_frame_buffer_image_available(),
			VK_NULL_HANDLE,
			&image_index
	);

	return image_index;
}
