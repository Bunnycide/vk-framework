namespace Config {
    constexpr static const bool ENABLE_VALIDATION_LAYERS = true;
}

struct Consts{
    static const int REQUIRED_QUEUE_COUNT  = 3;
    constexpr static const float QUEUE_PRIORITY_HIGHEST = 1.0f;
    constexpr static const float QUEUE_PRIORITY_LOWEST  = 0.0f;

    constexpr static const int NUM_VALIDATION_LAYERS = 1;
    constexpr static const char* VALIDATION_LAYERS[] = {
            "VK_LAYER_KHRONOS_validation"
    };

    constexpr static const int NUM_INSTANCE_EXTENSIONS = 2;
    constexpr static const char* INSTANCE_EXTENSIONS[] = {
            VK_KHR_SURFACE_EXTENSION_NAME,
            "VK_KHR_xcb_surface"
    };

    constexpr static const int NUM_DEVICE_EXTENSIONS = 1;
    constexpr static const char* DEVICE_EXTENSIONS[] = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
};

struct LightData{

};

struct ContextType {
    bool RAY_TRACING_ENABLED    = false;
    bool COMPUTE_ONLY           = false;
    bool GRAPHICS_ONLY          = true;
    bool GRAPHICS_AND_COMPUTE   = false;
};

struct BufferInfo{
    VkBuffer buffer;
    VkDeviceSize bufSz;
    VkBufferUsageFlags usage;
    VkDeviceMemory memoryObj;
    VkMemoryPropertyFlagBits memoryProperties;
};

struct BufferTransition {
    VkBuffer        buffer;
    VkAccessFlags   currentAccess;
    VkAccessFlags   newAccess;
    uint32_t        currentQueueFamily;
    uint32_t        newQueueFamily;
};

struct QueueInfo{
    std::vector<VkQueue> queues;
    uint32_t queueFamilyIndex;
    uint32_t queueCount;
};

struct WaitSemaphoreInfo {
    VkSemaphore Semaphore;
    VkPipelineStageFlags WaitingStage;
};

struct VulkanInstance{
    VkInstance                          instance;
    VkDevice                            logicalDevice;
    VkPhysicalDevice                    physicalDevice;
    std::vector<QueueInfo>              queueInfos;
    VkPhysicalDeviceMemoryProperties    physicalDeviceMemoryProperties;
};

struct VulkanSwapChain{
    VkSwapchainKHR swapchain;

    VkSurfaceKHR surface;
    VkFormat surfaceFormat;
    VkColorSpaceKHR colorSpace;
    uint32_t swapChainSize;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> imageViews;
    std::vector<VkFramebuffer> frameBuffers;
};

struct CommandPoolInfo{
    VkCommandPool commandPool;
    uint32_t queueFamilyIndex;
    std::vector<VkCommandBuffer> commandBuffers;
};

struct VulkanRender{
    VkPipeline          gfxPipeline;
    VkRenderPass        renderPass;
    VkPipelineLayout    pipelineLayout;
    // TODO: Please change this to an image later
    BufferInfo          depthResource;
};