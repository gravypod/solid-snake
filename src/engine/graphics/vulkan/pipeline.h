#ifndef ENGINE_PIPELINE_H
#define ENGINE_PIPELINE_H

#include <stdbool.h>
#include <src/engine/graphics/vulkan/shaders/shader.h>
#include "vulkan.h"

bool vulkan_pipeline_init(vulkan *v);

VkPipelineLayout vulkan_pipeline_layout_create(VkDevice logical_device);

/**
 * Create a graphics-focused vulkan pipeline
 *
 * @param logical_device
 * @param layout
 * @param render_pass
 * @param group
 * @return
 */
VkPipeline vulkan_pipeline_graphics_create(VkDevice logical_device, VkPipelineLayout layout, VkRenderPass render_pass, shader_group_t *group);

#endif
