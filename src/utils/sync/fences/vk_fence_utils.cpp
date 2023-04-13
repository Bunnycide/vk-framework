//
// Created by jae on 12/04/23.
//

#include "includes.h"

void H_createFence(VkDevice logicalDevice,
                   VkBool32 signaled,
                   VkFence& fence){

    VkFenceCreateInfo fenceCreateInfo{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};

    fenceCreateInfo.pNext = nullptr;
    fenceCreateInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

    VK_CHECK_RESULT(vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, & fence));
}

void H_waitForFences(VkDevice logicalDevice,
                     VkBool32 waitForAll ,
                     uint64_t timeOut,
                     std::vector<VkFence> fences){
    VK_CHECK_RESULT(vkWaitForFences(logicalDevice,
                                    static_cast<uint32_t>(fences.size()),
                                    &fences[0],
                                    waitForAll,
                                    timeOut))
}

void H_resetFences(VkDevice logicalDevice,
                   std::vector<VkFence>& fences){
    VK_CHECK_RESULT(vkResetFences(logicalDevice,
                                  static_cast<uint32_t>(fences.size()),
                                  &fences[0]))
}

void H_destroyFence(VkDevice logicalDevice,
                    VkFence fence){
    vkDestroyFence(logicalDevice, fence, nullptr);
}
