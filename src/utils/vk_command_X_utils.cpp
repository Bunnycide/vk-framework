//
// Created by jae on 05/04/23.
//

#include "includes.h"

void H_createCommandPool(VkDevice& logicalDevice,
                         CommandPoolInfo& commandPoolInfo){
    VkCommandPoolCreateInfo commandPoolCreateInfo{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    commandPoolCreateInfo.pNext = nullptr;
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolCreateInfo.queueFamilyIndex = commandPoolInfo.queueFamilyIndex;

    VK_CHECK_RESULT(vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &commandPoolInfo.commandPool))
}

void H_allocateCommandBuffers(VkDevice& logicalDevice,
                              uint32_t commandBuffersCount ,
                              CommandPoolInfo& commandPoolInfo){

    VkCommandBufferAllocateInfo commandBufferAllocateInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};

    commandPoolInfo.commandBuffers.resize(commandBuffersCount);

    commandBufferAllocateInfo.pNext = nullptr;
    commandBufferAllocateInfo.commandPool = commandPoolInfo.commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = commandBuffersCount;

    VK_CHECK_RESULT(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, commandPoolInfo.commandBuffers.data()));
}

void H_beginCommandBufferRecording(VkCommandBuffer commandBuffer,
                                   VkCommandBufferUsageFlags usageFlags,
                                   VkCommandBufferInheritanceInfo& secondaryCommandBufferInfo){
    VkCommandBufferBeginInfo commandBufferBeginInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    commandBufferBeginInfo.pNext = nullptr;
    commandBufferBeginInfo.flags = usageFlags;
    commandBufferBeginInfo.pInheritanceInfo = &secondaryCommandBufferInfo;

    VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo))
}

void H_endCommandBufferRecording(VkCommandBuffer commandBuffer){
    VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer))
}

void H_resetCommandBuffer(VkCommandBuffer commandBuffer,
                          VkBool32 releaseResources){
    VK_CHECK_RESULT(vkResetCommandBuffer(commandBuffer, releaseResources ? VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0))
}

void H_resetCommandPool(VkDevice logicalDevice,
                        VkCommandPool commandPool,
                        VkBool32 releaseResources){
    VK_CHECK_RESULT(vkResetCommandPool(logicalDevice, commandPool, releaseResources ? VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT : 0))
}

void H_freeCommandPool(VkDevice logicalDevice,
                       VkCommandPool& commandPool) {
    vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
}