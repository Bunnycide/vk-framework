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
                                   VkCommandBufferInheritanceInfo secondaryCommandBufferInfo){
    VkCommandBufferBeginInfo commandBufferBeginInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    commandBufferBeginInfo.pNext = nullptr;
    commandBufferBeginInfo.flags = usageFlags;
    commandBufferBeginInfo.pInheritanceInfo = &secondaryCommandBufferInfo;

    VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo))
}

void H_beginCommandBufferRecording(VkCommandBuffer commandBuffer,
                                   VkCommandBufferUsageFlags usageFlags){
    VkCommandBufferBeginInfo commandBufferBeginInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    commandBufferBeginInfo.pNext = nullptr;
    commandBufferBeginInfo.flags = usageFlags;
    commandBufferBeginInfo.pInheritanceInfo = nullptr;

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

void H_destroyCommandPool(VkDevice logicalDevice,
                          VkCommandPool& commandPool) {
    vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
}

void H_submitCommandBufferToQueue(VkQueue queue,
                                  std::vector<WaitSemaphoreInfo>& waitSemaphoreInfos,
                                  std::vector<VkSemaphore>& signalSemaphores,
                                  std::vector<VkCommandBuffer>& commandBuffer,
                                  VkFence fence){

    std::vector<VkSemaphore> wait_semaphore_handles;
    std::vector<VkPipelineStageFlags> wait_semaphore_stages;

    for(auto & waitSemaphoreInfo : waitSemaphoreInfos){
        wait_semaphore_handles.emplace_back(waitSemaphoreInfo.Semaphore);
        wait_semaphore_stages.emplace_back(waitSemaphoreInfo.WaitingStage);
    }

    VkSubmitInfo submit_info{VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submit_info.pNext = nullptr;
    submit_info.waitSemaphoreCount = static_cast<uint32_t>(wait_semaphore_handles.size());
    submit_info.pWaitSemaphores  = !wait_semaphore_handles.empty() ? wait_semaphore_handles.data() : nullptr;
    submit_info.pWaitDstStageMask = wait_semaphore_stages.empty() ? wait_semaphore_stages.data() : nullptr;
    submit_info.commandBufferCount = static_cast<uint32_t>(commandBuffer.size());
    submit_info.pCommandBuffers = !  commandBuffer.empty() ? commandBuffer.data() : nullptr;
    submit_info.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
    submit_info.pSignalSemaphores = !signalSemaphores.empty() ? signalSemaphores.data() : nullptr;

    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submit_info, fence))
}

void H_syncCommandBuffer(VkQueue firstQueue, VkQueue secondQueue,
                         VkFence secondFence,
                         std::vector<WaitSemaphoreInfo>& syncSemaphoreInfos,
                         std::vector<VkSemaphore>& secondSignalSemaphores,
                         std::vector<VkCommandBuffer>& firstBatchCommandBuffers,
                         std::vector<VkCommandBuffer>& secondBatchCommandBuffers){

    std::vector<WaitSemaphoreInfo> firstWaitSemaphoreInfos;
    for(WaitSemaphoreInfo& semaphoreInfo : syncSemaphoreInfos){
        firstWaitSemaphoreInfos.emplace_back(WaitSemaphoreInfo{
            semaphoreInfo.Semaphore,
            VK_PIPELINE_STAGE_NONE
        });
    }

    std::vector<VkSemaphore> firstSignalSemaphores;
    for(WaitSemaphoreInfo& semaphoreInfo : syncSemaphoreInfos){
        firstSignalSemaphores.emplace_back(semaphoreInfo.Semaphore);
    }

    H_submitCommandBufferToQueue(firstQueue,
                                 firstWaitSemaphoreInfos,
                                 firstSignalSemaphores,
                                 firstBatchCommandBuffers,
                                 VK_NULL_HANDLE);

    H_submitCommandBufferToQueue(secondQueue,
                                 syncSemaphoreInfos,
                                 secondSignalSemaphores,
                                 secondBatchCommandBuffers,
                                 secondFence);
}

void H_hasCommandBufferFinishedProcessing(VkDevice logicalDevice,
                                          VkBool32 waitForAll ,
                                          uint64_t timeOut,
                                          VkFence fence){
    H_waitForFences(logicalDevice, waitForAll, timeOut, {fence});
}

void waitForAllCommandsDoneQueue(VkQueue queue){
    vkQueueWaitIdle(queue);
}

void waitForAllCommandsDone(VkDevice logicalDevice){
    vkDeviceWaitIdle(logicalDevice);
}

void H_freeCommandBuffers(VkDevice logicalDevice,
                          VkCommandPool commandPool,
                          std::vector<VkCommandBuffer>& commandBuffers){
    vkFreeCommandBuffers(logicalDevice, commandPool, static_cast<uint32_t>(commandBuffers.size()),
                         &commandBuffers[0]);
}
