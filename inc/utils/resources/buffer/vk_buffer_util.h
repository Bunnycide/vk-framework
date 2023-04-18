//
// Created by jae on 12/04/23.
//

#ifndef VK_FRAMEWORK_VK_BUFFER_UTIL_H
#define VK_FRAMEWORK_VK_BUFFER_UTIL_H

void H_createBuffer(VkDevice, BufferInfo&);

void H_allocateAndBindMemoryObjectToBuffer(VkPhysicalDevice ,
                                           VkDevice ,
                                           VkPhysicalDeviceMemoryProperties ,
                                           BufferInfo& );

void H_setBufferMemoryBarrier(std::vector<BufferTransition> &,
                              VkPipelineStageFlags,
                              VkPipelineStageFlags,
                              VkCommandBuffer );

void H_createBufferView(VkDevice,
                        VkBuffer,
                        VkFormat,
                        VkDeviceSize,
                        VkDeviceSize,
                        VkBufferView &);

void H_createVertexBuffer(VkDevice,
                          VkDeviceSize,
                          BufferInfo&);

void H_copyToVertexBuffer(VkPhysicalDevice ,
                          VkDevice ,
                          VkPhysicalDeviceMemoryProperties ,
                          VkCommandBuffer ,
                          void* ,
                          uint32_t ,
                          BufferInfo& );

void H_freeBuffer(VkDevice, BufferInfo&);

#endif //VK_FRAMEWORK_VK_BUFFER_UTIL_H
