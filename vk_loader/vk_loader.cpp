#include <vector>
#include <string>
#include "inc/vk_loader.h"
#include "Log.h"

LIBRARY_TYPE vulkan_library;

PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = nullptr;

PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = nullptr;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = nullptr;
PFN_vkCreateInstance vkCreateInstance = nullptr;
PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion = nullptr;

PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = nullptr;
PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties = nullptr;
PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures = nullptr;
PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = nullptr;
PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2 = nullptr;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = nullptr;
PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties = nullptr;
PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties = nullptr;
PFN_vkCreateDevice vkCreateDevice = nullptr;
PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = nullptr;
PFN_vkDestroyInstance vkDestroyInstance = nullptr;

PFN_vkGetPhysicalDeviceSurfaceSupportKHR            vkGetPhysicalDeviceSurfaceSupportKHR        = nullptr;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR       vkGetPhysicalDeviceSurfaceCapabilitiesKHR   = nullptr;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR            vkGetPhysicalDeviceSurfaceFormatsKHR        = nullptr;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR       vkGetPhysicalDeviceSurfacePresentModesKHR   = nullptr;
PFN_vkDestroySurfaceKHR                             vkDestroySurfaceKHR                         = nullptr;

PFN_vkGetDeviceQueue                vkGetDeviceQueue                = nullptr;
PFN_vkDeviceWaitIdle                vkDeviceWaitIdle                = nullptr;
PFN_vkDestroyDevice                 vkDestroyDevice                 = nullptr;
PFN_vkCreateBuffer                  vkCreateBuffer                  = nullptr;
PFN_vkGetBufferMemoryRequirements   vkGetBufferMemoryRequirements   = nullptr;
PFN_vkAllocateMemory                vkAllocateMemory                = nullptr;
PFN_vkBindBufferMemory              vkBindBufferMemory              = nullptr;
PFN_vkCmdPipelineBarrier            vkCmdPipelineBarrier            = nullptr;
PFN_vkCreateImage                   vkCreateImage                   = nullptr;
PFN_vkGetImageMemoryRequirements    vkGetImageMemoryRequirements    = nullptr;
PFN_vkBindImageMemory               vkBindImageMemory               = nullptr;
PFN_vkCreateImageView               vkCreateImageView               = nullptr;
PFN_vkMapMemory                     vkMapMemory                     = nullptr;
PFN_vkFlushMappedMemoryRanges       vkFlushMappedMemoryRanges       = nullptr;
PFN_vkUnmapMemory                   vkUnmapMemory                   = nullptr;
PFN_vkCmdCopyBuffer                 vkCmdCopyBuffer                 = nullptr;
PFN_vkCmdCopyBufferToImage          vkCmdCopyBufferToImage          = nullptr;
PFN_vkCmdCopyImageToBuffer          vkCmdCopyImageToBuffer          = nullptr;
PFN_vkBeginCommandBuffer            vkBeginCommandBuffer            = nullptr;
PFN_vkEndCommandBuffer              vkEndCommandBuffer              = nullptr;
PFN_vkQueueSubmit                   vkQueueSubmit                   = nullptr;
PFN_vkDestroyImageView              vkDestroyImageView              = nullptr;
PFN_vkDestroyImage                  vkDestroyImage                  = nullptr;
PFN_vkDestroyBuffer                 vkDestroyBuffer                 = nullptr;
PFN_vkFreeMemory                    vkFreeMemory                    = nullptr;
PFN_vkCreateCommandPool             vkCreateCommandPool             = nullptr;
PFN_vkAllocateCommandBuffers        vkAllocateCommandBuffers        = nullptr;
PFN_vkCreateSemaphore               vkCreateSemaphore               = nullptr;
PFN_vkCreateFence                   vkCreateFence                   = nullptr;
PFN_vkWaitForFences                 vkWaitForFences                 = nullptr;
PFN_vkResetFences                   vkResetFences                   = nullptr;
PFN_vkDestroyFence                  vkDestroyFence                  = nullptr;
PFN_vkDestroySemaphore              vkDestroySemaphore              = nullptr;
PFN_vkResetCommandBuffer            vkResetCommandBuffer            = nullptr;
PFN_vkFreeCommandBuffers            vkFreeCommandBuffers            = nullptr;
PFN_vkResetCommandPool              vkResetCommandPool              = nullptr;
PFN_vkDestroyCommandPool            vkDestroyCommandPool            = nullptr;
PFN_vkCreateBufferView              vkCreateBufferView              = nullptr;
PFN_vkDestroyBufferView             vkDestroyBufferView             = nullptr;
PFN_vkQueueWaitIdle                 vkQueueWaitIdle                 = nullptr;
PFN_vkCreateSampler                 vkCreateSampler                 = nullptr;
PFN_vkCreateDescriptorSetLayout     vkCreateDescriptorSetLayout     = nullptr;
PFN_vkCreateDescriptorPool          vkCreateDescriptorPool          = nullptr;
PFN_vkAllocateDescriptorSets        vkAllocateDescriptorSets        = nullptr;
PFN_vkUpdateDescriptorSets          vkUpdateDescriptorSets          = nullptr;
PFN_vkCmdBindDescriptorSets         vkCmdBindDescriptorSets         = nullptr;
PFN_vkFreeDescriptorSets            vkFreeDescriptorSets            = nullptr;
PFN_vkResetDescriptorPool           vkResetDescriptorPool           = nullptr;
PFN_vkDestroyDescriptorPool         vkDestroyDescriptorPool         = nullptr;
PFN_vkDestroyDescriptorSetLayout    vkDestroyDescriptorSetLayout    = nullptr;
PFN_vkDestroySampler                vkDestroySampler                = nullptr;
PFN_vkCreateRenderPass              vkCreateRenderPass              = nullptr;
PFN_vkCreateFramebuffer             vkCreateFramebuffer             = nullptr;
PFN_vkDestroyFramebuffer            vkDestroyFramebuffer            = nullptr;
PFN_vkDestroyRenderPass             vkDestroyRenderPass             = nullptr;
PFN_vkCmdBeginRenderPass            vkCmdBeginRenderPass            = nullptr;
PFN_vkCmdNextSubpass                vkCmdNextSubpass                = nullptr;
PFN_vkCmdEndRenderPass              vkCmdEndRenderPass              = nullptr;
PFN_vkCreatePipelineCache           vkCreatePipelineCache           = nullptr;
PFN_vkGetPipelineCacheData          vkGetPipelineCacheData          = nullptr;
PFN_vkMergePipelineCaches           vkMergePipelineCaches           = nullptr;
PFN_vkDestroyPipelineCache          vkDestroyPipelineCache          = nullptr;
PFN_vkCreateGraphicsPipelines       vkCreateGraphicsPipelines       = nullptr;
PFN_vkCreateComputePipelines        vkCreateComputePipelines        = nullptr;
PFN_vkDestroyPipeline               vkDestroyPipeline               = nullptr;
PFN_vkDestroyEvent                  vkDestroyEvent                  = nullptr;
PFN_vkDestroyQueryPool              vkDestroyQueryPool              = nullptr;
PFN_vkCreateShaderModule            vkCreateShaderModule            = nullptr;
PFN_vkDestroyShaderModule           vkDestroyShaderModule           = nullptr;
PFN_vkCreatePipelineLayout          vkCreatePipelineLayout          = nullptr;
PFN_vkDestroyPipelineLayout         vkDestroyPipelineLayout         = nullptr;
PFN_vkCmdBindPipeline               vkCmdBindPipeline               = nullptr;
PFN_vkCmdSetViewport                vkCmdSetViewport                = nullptr;
PFN_vkCmdSetScissor                 vkCmdSetScissor                 = nullptr;
PFN_vkCmdBindVertexBuffers          vkCmdBindVertexBuffers          = nullptr;
PFN_vkCmdDraw                       vkCmdDraw                       = nullptr;
PFN_vkCmdDrawIndexed                vkCmdDrawIndexed                = nullptr;
PFN_vkCmdDispatch                   vkCmdDispatch                   = nullptr;
PFN_vkCmdCopyImage                  vkCmdCopyImage                  = nullptr;
PFN_vkCmdPushConstants              vkCmdPushConstants              = nullptr;
PFN_vkCmdClearColorImage            vkCmdClearColorImage            = nullptr;
PFN_vkCmdClearDepthStencilImage     vkCmdClearDepthStencilImage     = nullptr;
PFN_vkCmdBindIndexBuffer            vkCmdBindIndexBuffer            = nullptr;
PFN_vkCmdSetLineWidth               vkCmdSetLineWidth               = nullptr;
PFN_vkCmdSetDepthBias               vkCmdSetDepthBias               = nullptr;
PFN_vkCmdSetBlendConstants          vkCmdSetBlendConstants          = nullptr;
PFN_vkCmdExecuteCommands            vkCmdExecuteCommands            = nullptr;
PFN_vkCmdClearAttachments           vkCmdClearAttachments           = nullptr;
PFN_vkGetImageSubresourceLayout     vkGetImageSubresourceLayout     = nullptr;
//PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR     = nullptr;
//PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR               = nullptr;
//PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR               = nullptr;
//PFN_vkGetBufferDeviceAddress         vkGetBufferDeviceAddress                       = nullptr;
//PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR         = nullptr;

PFN_vkCreateSwapchainKHR    vkCreateSwapchainKHR               = nullptr;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR            = nullptr;
PFN_vkAcquireNextImageKHR   vkAcquireNextImageKHR              = nullptr;
PFN_vkQueuePresentKHR       vkQueuePresentKHR                  = nullptr;
PFN_vkDestroySwapchainKHR   vkDestroySwapchainKHR              = nullptr;

bool loadVulkan(){

#if defined _WIN32
    vulkan_library = LoadLibrary("vulkan-1.dll");
#elif defined __linux
    vulkan_library = dlopen("libvulkan.so", RTLD_NOW);
#endif

    if(vulkan_library == nullptr) {
        Log::error("Could not connect with vulkan runtime library");
        return false;
    }

    if(! loadExportedVulkanFunctions()) return false;
    if(! loadGlobalVulkanFunctions()) return false;

    return true;
}

bool loadExportedVulkanFunctions(){
#define EXPORTED_VULKAN_FUNCTION( name ) \
name = (PFN_##name)LoadFunction(vulkan_library, #name); \
if(name == nullptr){                     \
    Log::error("Could not load exported vulkan function named : "\
    #name);\
    return false;\
}
#include "inc/vk_functions_list.inl"

    return true;
}

bool loadGlobalVulkanFunctions() {
#define GLOBAL_LEVEL_VULKAN_FUNCTION(name) \
name = (PFN_##name)vkGetInstanceProcAddr(nullptr, #name); \
if(name == nullptr){                         \
    Log::error("Could not load global-level vulkan function named : "\
    #name);\
    return false;\
}
#include "inc/vk_functions_list.inl"

    return true;
}

bool loadInstanceVulkanFunctions(VkInstance vk_instance) {
#define INSTANCE_LEVEL_VULKAN_FUNCTION(name ) \
name = (PFN_##name)vkGetInstanceProcAddr(vk_instance, #name); \
if(name == nullptr){                           \
    Log::error("Could not load instance-level vulkan function named : "\
    #name);\
    return false;\
}
    #include "inc/vk_functions_list.inl"
    return true;
}

bool loadDeviceVulkanFunctions(VkDevice vk_device){
#define DEVICE_LEVEL_VULKAN_FUNCTION(name) \
name = (PFN_##name)vkGetDeviceProcAddr(vk_device, #name); \
if(name == nullptr){                       \
    Log::error("Could not load device-level vulkan function named : "\
    #name);\
    return false;                                       \
    }
#include "inc/vk_functions_list.inl"
    return true;
}

bool loadInstanceVulkanFunctionsFromExtensions(VkInstance vk_instance, std::vector<const char*>& enabled_extensions){
#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension ) \
for(auto& enabled_extension :  enabled_extensions ) {                    \
    if(std::string(enabled_extension) == std::string(extension)){        \
        name = (PFN_##name) vkGetInstanceProcAddr(vk_instance, #name);      \
        if(name == nullptr){                                             \
            Log::error("Could not load instance-level vulkan function named : "\
            #name);\
            return false;   \
        }\
    } \
}

#include "inc/vk_functions_list.inl"
    return true;
}

bool loadDeviceVulkanFunctionsFromExtensions(VkDevice vk_device, std::vector<const char *>& enabled_extensions){
#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension) \
for(auto& enabled_extension : enabled_extensions) {                   \
    if(std::string(enabled_extension) == std::string(extension)){    \
        name = (PFN_##name)vkGetDeviceProcAddr(vk_device, #name);    \
        if(name == nullptr){                                         \
        Log::error("Could not load device-level vulkan function named : "\
        #name);\
        return false; \
        }\
    }\
}
#include "inc/vk_functions_list.inl"
    return true;
}