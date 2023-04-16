#include "includes.h"

FrameWork::FrameWork(ContextType mContextType){
    contextType = mContextType;
}
FrameWork::~FrameWork()= default;

bool FrameWork::InitRenderEngine() {
    if(! loadVulkan()){
        Log::error("Failed to load vulkan funcs");
        return false;
    }

    if(Config::ENABLE_VALIDATION_LAYERS && ! H_checkValidationLayerSupport()){
        Log::error("Validation layers not found");
        return false;
    }
    if(! H_areInstanceExtensionsSupported()){
        Log::error("Instance level extensions not supported");
        return false;
    }

    H_createWindow(800, 600, "vk");

    //// Vulkan instance creation and func loading
    H_createVulkanInstance(vulkanInstance.instance, contextType);

    if(! loadInstanceVulkanFunctions(vulkanInstance.instance)){
        Log::error("Failed to load instance level vulkan funcs");
        return false;
    }
    std::vector<const char*> instanceExtensions(Consts::INSTANCE_EXTENSIONS, Consts::INSTANCE_EXTENSIONS+Consts::NUM_INSTANCE_EXTENSIONS);
    if(! loadInstanceVulkanFunctionsFromExtensions(vulkanInstance.instance, instanceExtensions)){
        Log::error("Failed to load instance level vulkan funcs from extensions");
        return false;
    }

    //// Pick a physical device
    std::vector<VkPhysicalDevice> physicalDevices;

    H_findAllPhysicalDevices(vulkanInstance.instance ,physicalDevices);
    H_pickSuitablePhysicalDevice(vulkanInstance.physicalDevice, physicalDevices);

    if(! H_areDeviceExtensionsSupported(vulkanInstance.physicalDevice)){
        Log::error("Could not load desired device extensions");
        return false;
    }

    //// Get physical device memory properties for later
    vkGetPhysicalDeviceMemoryProperties(vulkanInstance.physicalDevice,
                                        &vulkanInstance.physicalDeviceMemoryProperties);

    //// Vulkan create a logical device and load device level funcs
    H_createLogicalDevice(vulkanInstance.logicalDevice,
                          vulkanInstance.physicalDevice,
                          vulkanInstance.queueInfos);

    if(!loadDeviceVulkanFunctions(vulkanInstance.logicalDevice)){
        Log::error("Failed to load device level vulkan funcs");
        return false;
    }

    std::vector<const char*> deviceExtensions(Consts::DEVICE_EXTENSIONS, Consts::DEVICE_EXTENSIONS+Consts::NUM_DEVICE_EXTENSIONS);
    if(! loadDeviceVulkanFunctionsFromExtensions(vulkanInstance.logicalDevice, deviceExtensions)){
        Log::error("Failed to load device level vulkan funcs from extensions");
        return false;
    }

    //// Get device queues
    H_retriveDeviceQueues(vulkanInstance.logicalDevice, vulkanInstance.queueInfos);

    setupCommandPool();

    //// Create a render surface
    if(! H_createRenderSurface(vulkanInstance.instance,
                               vulkanSwapChain.surface)){
        Log::error("Failed to create render surface");
        return false;
    }

    //// Setup swap-chain stuff
    setupSwapChain();

    //// setup renderpass and gfx pipeline
    setupRenderPass();

    return true;
}

bool FrameWork::LoadScene(const char *path) {
    return false;
}

bool FrameWork::UseRenderType(RenderStyle) {
    return false;
}

bool FrameWork::LoadLightStyle(LightData *, const char *lightData) {
    return false;
}

void FrameWork::cleanup() {
    H_deleteRenderWindow();

    // Destroy command pool
    H_destroyCommandPool(vulkanInstance.logicalDevice,
                         gfxCommandPoolInfo.commandPool);
    H_destroyCommandPool(vulkanInstance.logicalDevice,
                         trxCommandPoolInfo.commandPool);

    // Destroy depth resources
    H_freeImage(vulkanInstance.logicalDevice, vulkanRender.depthResource);

    // Destroy gfx pipeline
    vkDestroyPipelineLayout(vulkanInstance.logicalDevice,
                            vulkanRender.pipelineLayout,
                            nullptr);

    // Destroy render pass
    vkDestroyRenderPass(vulkanInstance.logicalDevice,
                        vulkanRender.renderPass,
                        nullptr);

    // Destroy Image Views
    for(auto & imageView : vulkanSwapChain.imageViews){
        vkDestroyImageView(vulkanInstance.logicalDevice, imageView, nullptr);
    }

    // Destroy swap-chain
    vkDestroySwapchainKHR(vulkanInstance.logicalDevice, vulkanSwapChain.swapchain, nullptr);

    // Destroy surface
    vkDestroySurfaceKHR(vulkanInstance.instance, vulkanSwapChain.surface, nullptr);

    // Destroy logical device
    vkDestroyDevice(vulkanInstance.logicalDevice, nullptr);
    vulkanInstance.logicalDevice = nullptr;

    // Destroy instance
    vkDestroyInstance(vulkanInstance.instance, nullptr);
    vulkanInstance.instance = nullptr;
}

void FrameWork::mainLoop() {
    if(window == nullptr) return;

    while(!glfwWindowShouldClose(window)){
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void FrameWork::setupCommandPool(){
    //// Create command pool for gfx operations
    gfxCommandPoolInfo.queueFamilyIndex = vulkanInstance.queueInfos[IDX_GRAPHICS].queueFamilyIndex;

    H_createCommandPool(vulkanInstance.logicalDevice,
                        gfxCommandPoolInfo);

    //// Create command pool for transfer operations
    trxCommandPoolInfo.queueFamilyIndex = vulkanInstance.queueInfos[IDX_TRANSFER].queueFamilyIndex;

    H_createCommandPool(vulkanInstance.logicalDevice,
                        trxCommandPoolInfo);

    //// Allocate command buffers for gfx
    H_allocateCommandBuffers(vulkanInstance.logicalDevice,
                             2,
                             gfxCommandPoolInfo);

    //// Allocate command buffer for transfer
    H_allocateCommandBuffers(vulkanInstance.logicalDevice,
                             2,
                             trxCommandPoolInfo);
}

void FrameWork::setupSwapChain() {
    H_createSwapChain(vulkanInstance.physicalDevice,
                      vulkanInstance.logicalDevice,
                      vulkanSwapChain.surface,
                      800,
                      600,
                      vulkanInstance.queueInfos,
                      vulkanSwapChain.swapchain);

    H_getSwapChainImages(vulkanInstance.logicalDevice,
                         vulkanSwapChain.swapchain,
                         vulkanSwapChain.swapChainImages);

    VkSurfaceFormatKHR surfaceFormat = H_findPresentSurfaceImageFormat(vulkanInstance.physicalDevice, vulkanSwapChain.surface);
    vulkanSwapChain.colorSpace = surfaceFormat.colorSpace;
    vulkanSwapChain.surfaceFormat = surfaceFormat.format;

    H_createSwapChainImageViews(vulkanInstance.logicalDevice,
                                surfaceFormat,
                                vulkanSwapChain.swapChainImages,
                                vulkanSwapChain.imageViews);
}

void FrameWork::setupRenderPass() {

    Shader shader;
    shader.compileShader(vulkanInstance.logicalDevice,
                         "/shaders/shader.vert",
                         "/shaders/shader.frag");

    VkFormat depthFormat = H_findDepthFormat(vulkanInstance.physicalDevice);

    H_createDepthResources(vulkanInstance.physicalDevice,
                           vulkanInstance.logicalDevice,
                           vulkanInstance.physicalDeviceMemoryProperties,
                           gfxCommandPoolInfo.commandBuffers[0],
                           VkExtent3D {
                                   800,
                                   600,
                                   1
                           },
                           vulkanRender.depthResource);

    H_createPipelineLayout(vulkanInstance.logicalDevice,
                           vulkanRender.pipelineLayout,
                           shader.layout_bindings);

    H_createRenderPass(vulkanInstance.logicalDevice,
                       vulkanSwapChain.surfaceFormat,
                       depthFormat,
                       vulkanRender.renderPass);


    H_createRenderPipeline(vulkanInstance.logicalDevice,
                           800.0f, 600.0f,
                           shader,
                           vulkanRender.pipelineLayout,
                           vulkanRender.renderPass,
                           vulkanRender.gfxPipeline);
}
