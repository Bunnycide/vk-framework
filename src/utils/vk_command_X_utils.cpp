//
// Created by jae on 05/04/23.
//

#include "includes.h"

void H_createCommandBuffer(VkDevice& logicalDevice,
                           uint32_t queueFamilyIndex,
                           VkCommandPool& commandPool){
    VkCommandPoolCreateInfo commandPoolCreateInfo{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    commandPoolCreateInfo.pNext = nullptr;
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

    VK_CHECK_RESULT(vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &commandPool))
}

void H_allocateCommandBuffers(VkDevice& logicalDevice,
                              VkCommandPool& commandPool ,
                              uint32_t commandBuffersCount ,
                              std::vector<VkCommandBuffer>& commandBuffers){

    VkCommandBufferAllocateInfo commandBufferAllocateInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};

    commandBuffers.resize(commandBuffersCount);

    commandBufferAllocateInfo.pNext = nullptr;
    commandBufferAllocateInfo.commandPool = commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = commandBuffersCount;

    VK_CHECK_RESULT(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, commandBuffers.data()));
}