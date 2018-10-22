#include <vulkan/vulkan.h>
#include <stdio.h>
#include <lib/linmath/linmath.h>
#include <malloc.h>
#include "pipeline.h"
#include "window.h"
#include "src/engine/graphics/vulkan/shaders/shader.h"


static VkViewport viewport = {
		.x = 0.0f,
		.y = 0.0f,
		.width = 0, // This will be configured later in the code
		.height = 0, // This will be configured later in the code
		.minDepth = 0.0f,
		.maxDepth = 1.0f
};

static VkRect2D scissor = {
		.offset = {0, 0},
		.extent = {
				.height = 0,
				.width = 0
		} // This will be configured later in the code
};

static VkPipelineViewportStateCreateInfo viewport_state_creation_info = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports = &viewport,
		.scissorCount = 1,
		.pScissors = &scissor
};

static VkPipelineRasterizationStateCreateInfo rasterizer = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.lineWidth = 1.0f,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_CLOCKWISE,


		.depthBiasEnable = VK_FALSE,
		.depthBiasConstantFactor = 0.0f,
		.depthBiasClamp = 0.0f,
		.depthBiasSlopeFactor = 0.0f,
};

static VkPipelineMultisampleStateCreateInfo multisampling = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.sampleShadingEnable = VK_FALSE,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		.minSampleShading = 1.0f,
		.pSampleMask = NULL,
		.alphaToCoverageEnable = VK_FALSE,
		.alphaToOneEnable = VK_FALSE
};

static VkPipelineColorBlendAttachmentState color_blending_attachment_state = {
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
						  VK_COLOR_COMPONENT_A_BIT,
		.blendEnable = VK_FALSE,
		.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
		.colorBlendOp = VK_BLEND_OP_ADD,
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
		.alphaBlendOp = VK_BLEND_OP_ADD,
};

static VkPipelineColorBlendStateCreateInfo color_blending = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = 1,
		.pAttachments = &color_blending_attachment_state,
		.blendConstants = {
				0.0f,
				0.0f,
				0.0f,
				0.0f
		}
};

bool vulkan_pipeline_render_pass_init(vulkan *v)
{
	VkSubpassDependency dependency = {
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = 0,
			.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			.srcAccessMask = 0,
			.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
	};

	VkAttachmentDescription color_attachment = {
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.format = v->swapchain.format.format,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};
	VkAttachmentReference reference = {
			.attachment = 0,
			.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	};
	VkSubpassDescription subpass = {
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,

			.colorAttachmentCount = 1,
			.pColorAttachments = &reference,
	};

	VkRenderPassCreateInfo render_pass_info = {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,

			.attachmentCount = 1,
			.pAttachments = &color_attachment,

			.subpassCount = 1,
			.pSubpasses = &subpass,

			.dependencyCount = 1,
			.pDependencies = &dependency
	};


	return vkCreateRenderPass(
			v->devices.logical_device,
			&render_pass_info,
			NULL,
			&v->pipelines.render_pass
	) == VK_SUCCESS;
}

VkPipelineLayout vulkan_pipeline_layout_create(VkDevice logical_device)
{
	VkPipelineLayout created_layout = VK_NULL_HANDLE;
	VkPipelineLayoutCreateInfo request = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.setLayoutCount = 0,
			.pSetLayouts = NULL,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = NULL,
	};

	vkCreatePipelineLayout(logical_device, &request, NULL, &created_layout);

	return created_layout;
}

VkPipeline vulkan_pipeline_graphics_create(
		VkDevice logical_device, VkPipelineLayout layout, VkRenderPass render_pass,
		shader_group_t *group
)
{
	VkPipeline created_pipeline = VK_NULL_HANDLE;
	VkGraphicsPipelineCreateInfo create_info = {
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.stageCount = group->num_shaders,
			.pStages = group->stages,

			.pVertexInputState = &group->vertex_input_state_create_info,
			.pInputAssemblyState = &group->input_assembly_state,
			.pViewportState = &viewport_state_creation_info,
			.pRasterizationState = &rasterizer,
			.pMultisampleState = &multisampling,
			.pDepthStencilState = NULL, // Optional
			.pColorBlendState = &color_blending,
			.pDynamicState = NULL, // &dynamic_state_create_info, // Optional

			.layout = layout,

			.renderPass = render_pass,
			.subpass = 0,

			.basePipelineHandle = VK_NULL_HANDLE,
	};

	// Pass to vulkan drivers. We don't need to check VK_SUCCESS because
	//
	vkCreateGraphicsPipelines(
			logical_device,
			VK_NULL_HANDLE,
			1,
			&create_info,
			NULL,
			&created_pipeline
	);

	return created_pipeline;
}

bool vulkan_pipeline_init(vulkan *v)
{

	// configure viewport size
	viewport.width = vulkan_window_width_get();
	viewport.height = vulkan_window_height_get();

	// configure swapchain extent
	scissor.extent = v->swapchain.extent;

	if (!vulkan_pipeline_render_pass_init(v)) {
		printf("Failed to initialize render pass\n");
		return false;
	}

	return true;
}
