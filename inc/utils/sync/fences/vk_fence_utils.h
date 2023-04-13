//
// Created by jae on 12/04/23.
//

#ifndef VK_FRAMEWORK_VK_FENCE_UTILS_H
#define VK_FRAMEWORK_VK_FENCE_UTILS_H

void H_createFence(VkDevice,
                   VkBool32,
                   VkFence&);

void H_waitForFences(VkDevice,
                    VkBool32,
                    uint64_t,
                    std::vector<VkFence>);

void H_resetFences(VkDevice,
                   std::vector<VkFence>&);

void H_destroyFence(VkDevice,
                    VkFence);

#endif //VK_FRAMEWORK_VK_FENCE_UTILS_H
