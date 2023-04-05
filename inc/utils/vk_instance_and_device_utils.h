
#ifndef VK_FRAMEWORK_VK_VALIDATION_LAYER_UTILS_H
#define VK_FRAMEWORK_VK_VALIDATION_LAYER_UTILS_H

void H_createVulkanInstance(VkInstance&,
                            ContextType&);

void H_findAllPhysicalDevices(VkInstance& ,
                              std::vector<VkPhysicalDevice>&);
void H_pickSuitablePhysicalDevice(VkPhysicalDevice& ,
                                  std::vector<VkPhysicalDevice>&);

void H_createLogicalDevice(VkDevice&,
                           VkPhysicalDevice&,
                           std::vector<QueueInfo>&);

void H_retriveDeviceQueues(VkDevice&,
                           std::vector<QueueInfo>&);

#endif //#ifndef VK_FRAMEWORK_VK_VALIDATION_LAYER_UTILS_H
#define VK_FRAMEWORK_VK_VALIDATION_LAYER_UTILS_H