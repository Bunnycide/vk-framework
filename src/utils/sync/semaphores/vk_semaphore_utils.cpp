//
// Created by jae on 12/04/23.
//

#include "includes.h"

void H_createSemaphore(VkDevice logicalDevice,
                       VkSemaphore& semaphore){
    VkSemaphoreCreateInfo semaphoreCreateInfo{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    semaphoreCreateInfo.pNext = nullptr;
    semaphoreCreateInfo.flags = 0;

    VK_CHECK_RESULT(vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &semaphore))
}

void H_destroySemaphore(VkDevice logicalDevice, VkSemaphore semaphore){
    vkDestroySemaphore(logicalDevice, semaphore, nullptr);
}

