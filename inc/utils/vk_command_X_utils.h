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

void H_freeCommandPool(VkDevice,
                       VkCommandPool&);

#endif //VK_FRAMEWORK_VK_COMMAND_X_UTILS_H
