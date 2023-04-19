#include "includes.h"
#include "vk_framework.h"

#if defined _WIN32
    FrameWork::FrameWork(ContextType, WindowWindows) {

    }
#elif defined __linux
    FrameWork::FrameWork(ContextType mcontextType, WindowLinux& mWindowLinux) : contextType(mcontextType), window(mWindowLinux) {
        window.setLooper(reinterpret_cast<DrawLooper *>(this));
    }
#elif defined __ANDROID__
FrameWork::FrameWork(ContextType, WindowAndroid) {

    }
#endif

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
    window.createWindowSurface(vulkanInstance.instance);
    vulkanSwapChain.surface = window.getVkSurface();

    //// Setup swap-chain stuff
    setupSwapChain();

    //// setup renderpass and gfx pipeline
    setupRenderPass();

    //// Create frame buffers
    H_createSwapChainFrameBuffers(vulkanInstance.logicalDevice,
                                  gpuRenderPass.vulkanRender.depthResource,
                                  gpuRenderPass.vulkanRender.renderPass,
                                  vulkanSwapChain.swapChainImageViews,
                                  vulkanSwapChain.swapChainFrameBuffers);
    VkExtent2D size = window.getWindowSize();
    drawSetup.initDraw(vulkanInstance.logicalDevice,
                       (int)size.width, (int)size.height, (int)vulkanSwapChain.swapChainImages.size());

    setupGeom();
//    recordCommandsAndDraw();

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
    window.deleteWindow();

    // Destroy command pool
    H_destroyCommandPool(vulkanInstance.logicalDevice,
                         gfxCommandPoolInfo.commandPool);
    H_destroyCommandPool(vulkanInstance.logicalDevice,
                         trxCommandPoolInfo.commandPool);

    // Destroy Descriptor set data
    H_destroyDescriptorData(vulkanInstance.logicalDevice, gpuRenderPass.descriptorData);

    // Destroy depth resources
    H_freeImage(vulkanInstance.logicalDevice, gpuRenderPass.vulkanRender.depthResource);

    // Destroy gfx pipeline
    vkDestroyPipelineLayout(vulkanInstance.logicalDevice,
                            gpuRenderPass.vulkanRender.pipelineLayout,
                            nullptr);
    // Destroy gfx pipeline
    vkDestroyPipeline(vulkanInstance.logicalDevice,
                      gpuRenderPass.vulkanRender.gfxPipeline,
                      nullptr);

    // Destroy render pass
    vkDestroyRenderPass(vulkanInstance.logicalDevice,
                        gpuRenderPass.vulkanRender.renderPass,
                        nullptr);

    // Destroy frame buffers

    H_destroyFrameBuffers(vulkanInstance.logicalDevice,
                          vulkanSwapChain.swapChainFrameBuffers);

    // Destroy Image Views
    for(auto & imageView : vulkanSwapChain.swapChainImageViews){
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

void FrameWork::draw() {

}

void FrameWork::mainLoop() {
    drawFrame();
    window.loop(0.0f);
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
    VkExtent2D size = window.getWindowSize();

    H_createSwapChain(vulkanInstance.physicalDevice,
                      vulkanInstance.logicalDevice,
                      vulkanSwapChain.surface,
                      size.width,
                      size.height,
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
                                vulkanSwapChain.swapChainImageViews);
}

void FrameWork::setupRenderPass() {

    VkExtent2D size = window.getWindowSize();

    Log::info("Story %d %d", size.width, size.height);

    gpuRenderPass.createRenderPass(vulkanInstance.logicalDevice,
                                   vulkanInstance.physicalDevice,
                                   vulkanInstance.physicalDeviceMemoryProperties,
                                   vulkanSwapChain.surfaceFormat,
                                   VkExtent3D{
                                           size.width,
                                           size.height,
                                           1
                                    },
                                    gfxCommandPoolInfo.commandBuffers[0],
                                   "/shaders/shader.vert",
                                   "/shaders/shader.frag");

}

void FrameWork::setupGeom(){
    const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };

    H_createVertexBuffer(vulkanInstance.logicalDevice, sizeof(Vertex) * vertices.size(),
                         vertexBufferInfo);

    H_allocateAndBindMemoryObjectToBuffer(vulkanInstance.physicalDevice,
                                          vulkanInstance.logicalDevice,
                                          vulkanInstance.physicalDeviceMemoryProperties,
                                          vertexBufferInfo);

    H_copyToVertexBuffer(vulkanInstance.physicalDevice,
                         vulkanInstance.logicalDevice,
                         vulkanInstance.physicalDeviceMemoryProperties,
                         gfxCommandPoolInfo.commandBuffers[0],
                         (void *) vertices.data(),
                         vulkanInstance.queueInfos[IDX_TRANSFER].queueFamilyIndex,
                         vertexBufferInfo);

    const std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0
    };

    H_createVertexBuffer(vulkanInstance.logicalDevice,
                         indices.size() * sizeof(uint16_t),
                         indexBufferInfo);

    H_allocateAndBindMemoryObjectToBuffer(vulkanInstance.physicalDevice,
                                          vulkanInstance.logicalDevice,
                                          vulkanInstance.physicalDeviceMemoryProperties,
                                          indexBufferInfo);

    H_copyToVertexBuffer(vulkanInstance.physicalDevice,
                         vulkanInstance.logicalDevice,
                         vulkanInstance.physicalDeviceMemoryProperties,
                         gfxCommandPoolInfo.commandBuffers[0],
                         (void *) indices.data(),
                         vulkanInstance.queueInfos[IDX_TRANSFER].queueFamilyIndex,
                         indexBufferInfo);

    texture.create(vulkanInstance.physicalDevice,
                   vulkanInstance.logicalDevice,
                   vulkanInstance.queueInfos[IDX_GRAPHICS].queues[0],
                   gfxCommandPoolInfo.commandBuffers[1],
                   vulkanInstance.physicalDeviceMemoryProperties,
                   "/texture/oasis.png");
}

void FrameWork::setupSyncObjects(){

}

void FrameWork::recordCommands(){
    H_resetCommandBuffer(gfxCommandPoolInfo.commandBuffers[0], false);
    H_beginCommandBufferRecording(gfxCommandPoolInfo.commandBuffers[0],
                                  VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    vkCmdBeginRenderPass(gfxCommandPoolInfo.commandBuffers[0],
                         &drawSetup.renderPassBeginInfo,
                         VK_SUBPASS_CONTENTS_INLINE);

    vkCmdSetViewport(gfxCommandPoolInfo.commandBuffers[0],
                     0, 1, &drawSetup.viewport);

    vkCmdSetScissor(gfxCommandPoolInfo.commandBuffers[0],
                    0, 1, &drawSetup.scissor);

    vkCmdBindVertexBuffers(gfxCommandPoolInfo.commandBuffers[0],
                           0, 1, &vertexBufferInfo.buffer,
                           &drawSetup.offset);

    vkCmdBindIndexBuffer(gfxCommandPoolInfo.commandBuffers[0],
                         indexBufferInfo.buffer,
                         0,
                         VK_INDEX_TYPE_UINT16);

    vkCmdBindPipeline(gfxCommandPoolInfo.commandBuffers[0],
                      VK_PIPELINE_BIND_POINT_GRAPHICS,
                      gpuRenderPass.vulkanRender.gfxPipeline);

    vkCmdBindDescriptorSets(gfxCommandPoolInfo.commandBuffers[0],
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            gpuRenderPass.vulkanRender.pipelineLayout,
                            0,
                            gpuRenderPass.descriptorData.descriptorSets.size(),
                            gpuRenderPass.descriptorData.descriptorSets.data(),
                            0,
                            nullptr);

    vkCmdDrawIndexed(gfxCommandPoolInfo.commandBuffers[0],
                     6,
                     1,
                     0,
                     0,
                     0);

    vkCmdEndRenderPass(gfxCommandPoolInfo.commandBuffers[0]);

    H_endCommandBufferRecording(gfxCommandPoolInfo.commandBuffers[0]);
}

void FrameWork::drawFrame() {

    drawSetup.startDraw(vulkanInstance.logicalDevice,
                        gpuRenderPass.vulkanRender.renderPass,
                        vulkanSwapChain.swapChainFrameBuffers,
                        vulkanSwapChain.swapchain);

    recordCommands();

    drawSetup.endDraw(vulkanInstance.logicalDevice,
                      vulkanSwapChain.swapchain,
                      vulkanInstance.queueInfos[IDX_GRAPHICS].queues[0],
                      gfxCommandPoolInfo.commandBuffers[0]);
}



