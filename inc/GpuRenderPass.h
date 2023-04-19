//
// Created by jae on 18/04/23.
//

#ifndef VK_FRAMEWORK_GPURENDERPASS_H
#define VK_FRAMEWORK_GPURENDERPASS_H


class GpuRenderPass {
public:
    GpuRenderPass()     = default;
    ~GpuRenderPass()    = default;
    void createRenderPass(VkDevice ,
                          VkPhysicalDevice ,
                          VkPhysicalDeviceMemoryProperties ,
                          VkFormat ,
                          VkExtent3D ,
                          VkCommandBuffer& ,
                          const char* , const char* );

    VulkanRender        vulkanRender{};
    DescriptorData      descriptorData{};

private:
    Shader              shader{};
};


#endif //VK_FRAMEWORK_GPURENDERPASS_H
