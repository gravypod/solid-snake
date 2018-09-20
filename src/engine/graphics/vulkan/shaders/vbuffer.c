#include <string.h>
#include <src/engine/graphics/vulkan/memory.h>
#include "vbuffer.h"

const VkMemoryPropertyFlags vbuffer_memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

bool vulkan_vbuffer_allocate(vulkan *v, vbuffer *buffer, uint32_t element_size, uint32_t num_elements,
                             bool map_memory_region) {
    const uint32_t size = element_size * num_elements;

    // Clear out old vbuffer
    memset(buffer, 0, sizeof(vbuffer));

    // Engine metadata
    buffer->element_size = element_size;
    buffer->num_elements = num_elements;

    // Set up buffer dimension info
    buffer->info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer->info.size = size;
    buffer->info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buffer->info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(v->devices.logical_device, &buffer->info, NULL, &buffer->buffer) != VK_SUCCESS) {
        return false;
    }

    vkGetBufferMemoryRequirements(v->devices.logical_device, buffer->buffer, &buffer->required_memory);

    if (!vulkan_memory_allocate(v, buffer->required_memory.memoryTypeBits, vbuffer_memory_properties, size, &buffer->memory)) {
        return false;
    }

    // Bind the memory to the new buffer
    if (vkBindBufferMemory(v->devices.logical_device, buffer->buffer, buffer->memory, 0) != VK_SUCCESS) {
        return false;
    }

    if (map_memory_region) {
        // Map memory location into trapped page (I guess?)
        if (vkMapMemory(v->devices.logical_device, buffer->memory, 0, buffer->info.size, 0, &buffer->mapped_memory) != VK_SUCCESS) {
            return false;
        }
    }

    return true;
}
