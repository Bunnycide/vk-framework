//
// Created by jae on 12/04/23.
//

#ifndef VK_FRAMEWORK_VK_BUFFER_UTIL_H
#define VK_FRAMEWORK_VK_BUFFER_UTIL_H

void H_createBuffer(VkDevice, BufferInfo&);

void H_allocateAndBindMemoryObject(VkPhysicalDevice physicalDevice, BufferInfo bufferInfo);

void H_setBufferMemoryBarrier(std::vector<BufferTransition>&);

void H_beginCommandBufferRecording(VkCommandBuffer,
                                   VkCommandBufferUsageFlags,
                                   VkCommandBufferInheritanceInfo&);

void H_endCommandBufferRecording(VkCommandBuffer);

void H_resetCommandBuffer(VkCommandBuffer, VkBool32);

void H_resetCommandPool(VkCommandPool, VkBool32);

void H_freeBuffer(VkDevice, BufferInfo&);

#endif //VK_FRAMEWORK_VK_BUFFER_UTIL_H
