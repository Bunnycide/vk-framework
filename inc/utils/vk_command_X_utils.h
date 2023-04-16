//
// Created by jae on 05/04/23.
//

#ifndef VK_FRAMEWORK_VK_COMMAND_X_UTILS_H
#define VK_FRAMEWORK_VK_COMMAND_X_UTILS_H

void H_createCommandPool(VkDevice& logicalDevice,
                         CommandPoolInfo& commandPoolInfo);

void H_allocateCommandBuffers(VkDevice& logicalDevice,
                              uint32_t commandBuffersCount ,
                              CommandPoolInfo& commandPoolInfo);

void H_destroyCommandPool(VkDevice,
                          VkCommandPool&);

void H_submitCommandBufferToQueue(VkQueue,
                                  std::vector<WaitSemaphoreInfo>&,
                                  std::vector<VkSemaphore>&,
                                  std::vector<VkCommandBuffer>&,
                                  VkFence);

void H_syncCommandBuffer(VkQueue firstQueue, VkQueue secondQueue,
                         std::vector<WaitSemaphoreInfo>& syncSemaphores,
                         std::vector<VkCommandBuffer>& firstBatchCommandBuffers,
                         std::vector<VkCommandBuffer>& secondBatchCommandBuffers);

void H_hasCommandBufferFinishedProcessing(VkQueue);

void waitForAllCommandsDone(VkDevice);

void H_freeCommandBuffers(VkDevice,
                          VkCommandPool,
                          std::vector<VkCommandBuffer>&);

void H_beginCommandBufferRecording(VkCommandBuffer,
                                   VkCommandBufferUsageFlags,
                                   VkCommandBufferInheritanceInfo);

void H_beginCommandBufferRecording(VkCommandBuffer,
                                   VkCommandBufferUsageFlags);

void H_endCommandBufferRecording(VkCommandBuffer);

void H_resetCommandBuffer(VkCommandBuffer, VkBool32);

void H_resetCommandPool(VkCommandPool, VkBool32);


#endif //VK_FRAMEWORK_VK_COMMAND_X_UTILS_H
