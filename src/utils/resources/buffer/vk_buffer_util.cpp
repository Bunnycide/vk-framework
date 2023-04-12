#include "includes.h"

void H_createBuffer(VkDevice logicalDevice, BufferInfo& bufferInfo){
    VkBufferCreateInfo createInfo{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};

    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.size = bufferInfo.bufSz;
    createInfo.usage = bufferInfo.usage;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.pQueueFamilyIndices = nullptr;

    VK_CHECK_RESULT(vkCreateBuffer(logicalDevice, &createInfo, nullptr, &bufferInfo.buffer))
}

void H_allocateAndBindMemoryObject(VkPhysicalDevice physicalDevice,
                                   VkDevice logicalDevice,
                                   VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
                                   BufferInfo& bufferInfo){

    // Required type of memory for the buffer
    VkMemoryRequirements memory_requirements;
    vkGetBufferMemoryRequirements(logicalDevice, bufferInfo.buffer, &memory_requirements);

    // Initialize memory handle
    bufferInfo.memoryObj = VK_NULL_HANDLE;

    for(uint32_t type = 0; type < physicalDeviceMemoryProperties.memoryTypeCount; ++type){

        /**
         *First section to decide if type index is correct
         *(memory_requirements.memoryTypeBits & (1 << type))
         *
         * Second section checks for memory type being present
         *
         * ((physical_device_memory_properties.memoryTypes[type].propertyFlags &
         *    bufferInfo.memoryProperties) == bufferInfo.memoryProperties)
         */

        if( (memory_requirements.memoryTypeBits & (1 << type)) &&
            ((physicalDeviceMemoryProperties.memoryTypes[type].propertyFlags &
              bufferInfo.memoryProperties) == bufferInfo.memoryProperties) ){

            VkMemoryAllocateInfo buffer_memory_allocate_info = {
                    VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                    nullptr,
                    memory_requirements.size,
                    type
            };

            VkResult result = vkAllocateMemory(logicalDevice, &buffer_memory_allocate_info, nullptr, &bufferInfo.memoryObj);

            if(result == VK_SUCCESS){
                break;
            }
        }
    }

    if(VK_NULL_HANDLE == bufferInfo.memoryObj){
        Log::error("Could not allocate memory for buffer");
        return;
    }

    VK_CHECK_RESULT(vkBindBufferMemory(logicalDevice, bufferInfo.buffer, bufferInfo.memoryObj, 0))
}

void H_setBufferMemoryBarrier(std::vector<BufferTransition> &bufferTransitions){
    std::vector<VkBufferMemoryBarrier> buffer_memory_barriers;

    for(auto& bufferTransition : bufferTransitions){
        buffer_memory_barriers.push_back({
            VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
            nullptr,
            bufferTransition.currentAccess,
            bufferTransition.newAccess,
            bufferTransition.currentQueueFamily,
            bufferTransition.newQueueFamily,
            bufferTransition.buffer,
            0,
            VK_WHOLE_SIZE
        });
    }
}

void H_freeBuffer(VkDevice logicalDevice, BufferInfo& bufferInfo){
    vkDestroyBuffer(logicalDevice, bufferInfo.buffer, nullptr);
}