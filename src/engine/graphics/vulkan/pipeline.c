#include <vulkan/vulkan.h>
#include <stdio.h>
#include <lib/linmath/linmath.h>
#include "pipeline.h"
#include "window.h"
#include "src/engine/graphics/vulkan/shaders/shader.h"


#define NUM_VERTEX_INPUT_BINDING_ATTRIBUTE_DESCRIPTIONS 1
static VkVertexInputAttributeDescription vertex_input_binding_attrbute_descriptions[NUM_VERTEX_INPUT_BINDING_ATTRIBUTE_DESCRIPTIONS] = {
        {
            .binding = 0,
            .offset = 0,
            .format = VK_FORMAT_R32G32_SFLOAT,
            .location = 0
        }
};

#define NUM_VERTEX_INPUT_BINDING_DESCRIPTIONS 1
static VkVertexInputBindingDescription vertex_input_binding_description[NUM_VERTEX_INPUT_BINDING_DESCRIPTIONS] = {
        {
                .binding = 0,
                .stride = sizeof(vec2),
                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
        }
};

static VkPipelineVertexInputStateCreateInfo vertext_input_state_create_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = NUM_VERTEX_INPUT_BINDING_DESCRIPTIONS,
        .pVertexBindingDescriptions = vertex_input_binding_description,
        .vertexAttributeDescriptionCount = NUM_VERTEX_INPUT_BINDING_ATTRIBUTE_DESCRIPTIONS,
        .pVertexAttributeDescriptions = vertex_input_binding_attrbute_descriptions
};


static VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
        .primitiveRestartEnable = VK_FALSE
};

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

const VkDynamicState dynamic_states[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH,
};

static VkPipelineDynamicStateCreateInfo dynamic_state_create_info = {
        .sType= VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = 2,
        .pDynamicStates = dynamic_states
};

bool vulkan_pipeline_render_pass_init(vulkan *v) {


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

bool vulkan_pipeline_layout_init(vulkan *v) {
    VkPipelineLayoutCreateInfo request = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 0,
            .pSetLayouts = NULL,
            .pushConstantRangeCount = 0,
            .pPushConstantRanges = NULL,
    };

    return vkCreatePipelineLayout(v->devices.logical_device, &request, NULL, &v->pipelines.layout) == VK_SUCCESS;
}

bool vulkan_pipeline_graphics_init(vulkan *v) {

#define NUM_SHADER_STAGES 3
    VkPipelineShaderStageCreateInfo stages[NUM_SHADER_STAGES];

    if (!vulkan_shader_pipeline_shader_stage_get(VULKAN_SHADER_VERTEX_TEST, &stages[0])) {
        return false;
    }

    if (!vulkan_shader_pipeline_shader_stage_get(VULKAN_SHADER_GEOMETRY_TEST, &stages[1])) {
        return false;
    }

    if (!vulkan_shader_pipeline_shader_stage_get(VULKAN_SHADER_FRAGMENT_TEST, &stages[2])) {
        return false;
    }


    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = NUM_SHADER_STAGES,
            .pStages = stages,

            .pVertexInputState = &vertext_input_state_create_info,
            .pInputAssemblyState = &input_assembly_state_create_info,
            .pViewportState = &viewport_state_creation_info,
            .pRasterizationState = &rasterizer,
            .pMultisampleState = &multisampling,
            .pDepthStencilState = NULL, // Optional
            .pColorBlendState = &color_blending,
            .pDynamicState = NULL, // &dynamic_state_create_info, // Optional


            .layout = v->pipelines.layout,

            .renderPass = v->pipelines.render_pass,
            .subpass = 0,


            .basePipelineHandle = VK_NULL_HANDLE,
            //.basePipelineIndex = -1,
    };

    return vkCreateGraphicsPipelines(
            v->devices.logical_device,
            VK_NULL_HANDLE,
            1,
            &graphics_pipeline_create_info,
            NULL,
            &v->pipelines.graphics
    ) == VK_SUCCESS;
}

bool vulkan_pipeline_init(vulkan *v) {

    // configure viewport size
    viewport.width = vulkan_window_width_get();
    viewport.height = vulkan_window_height_get();

    // configure swapchain extent
    scissor.extent = v->swapchain.extent;


    if (!vulkan_pipeline_layout_init(v)) {
        printf("Failed to create pipeline layout\n");
        return false;
    }

    if (!vulkan_pipeline_render_pass_init(v)) {
        printf("Failed to initialize render pass\n");
        return false;
    }

    if (!vulkan_pipeline_graphics_init(v)) {
        printf("Failed to initialize graphics pipeline\n");
        return false;
    }

    return true;
}
