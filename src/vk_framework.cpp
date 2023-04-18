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
                                  vulkanRender.depthResource,
                                  vulkanRender.renderPass,
                                  vulkanSwapChain.swapChainImageViews,
                                  vulkanSwapChain.swapChainFrameBuffers);

    setupSyncObjects();
    setupGeom();

    drawFrame();

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
    H_destroyDescriptorData(vulkanInstance.logicalDevice, descriptorData);

    // Destroy depth resources
    H_freeImage(vulkanInstance.logicalDevice, vulkanRender.depthResource);

    // Destroy gfx pipeline
    vkDestroyPipelineLayout(vulkanInstance.logicalDevice,
                            vulkanRender.pipelineLayout,
                            nullptr);
    // Destroy gfx pipeline
    vkDestroyPipeline(vulkanInstance.logicalDevice,
                      vulkanRender.gfxPipeline,
                      nullptr);

    // Destroy render pass
    vkDestroyRenderPass(vulkanInstance.logicalDevice,
                        vulkanRender.renderPass,
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
    // TODO::Update renderpass here
}

void FrameWork::mainLoop() {
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
                                vulkanSwapChain.swapChainImageViews);

}

void FrameWork::setupRenderPass() {

    int *width, *height;

    window.getWindowSize(*width, *height);

    gpuRenderPass.createRenderPass(vulkanInstance.logicalDevice,
                                   vulkanInstance.physicalDevice,
                                   vulkanInstance.physicalDeviceMemoryProperties,
                                   vulkanSwapChain.surfaceFormat,
                                   VkExtent3D{
                                        width,
                                        height
                                    },
            VkCommandBuffer& commandBuffer,
    const char* vertPath, const char* fragPath);

    Shader shader;
    shader.compileShader(vulkanInstance.logicalDevice,
                         "/shaders/shader.vert",
                         "/shaders/shader.frag");

    VkDescriptorSetLayout layout;;

    H_createDescriptorSetLayout(vulkanInstance.logicalDevice,
                                shader.layout_bindings,
                                layout);

    descriptorData.layouts = { layout };

    std::vector<VkDescriptorPoolSize> descPoolSizes =
            H_extractDescriptorSetTypes(shader.layout_bindings);

    VkFormat depthFormat = H_findDepthFormat(vulkanInstance.physicalDevice);

    H_createDescriptorPool(vulkanInstance.logicalDevice,
                           descPoolSizes,
                           1,
                           VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
                           descriptorData.descriptorPool);

    H_allocateDescriptorSets(vulkanInstance.logicalDevice,
                             descriptorData.descriptorPool,
                             descriptorData.layouts,
                             descriptorData.descriptorSets);

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
                           descriptorData.layouts);

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
}

void FrameWork::setupSyncObjects(){
    inFlightFences.resize(vulkanSwapChain.swapChainImages.size());
    imgAvailSemaphores.resize(vulkanSwapChain.swapChainImages.size());
    signalSemaphores.resize(vulkanSwapChain.swapChainImages.size());

    for(int i = 0; i < inFlightFences.size(); i++){
        H_createFence(vulkanInstance.logicalDevice, false, inFlightFences[i]);
        H_createSemaphore(vulkanInstance.logicalDevice, imgAvailSemaphores[i]);
        H_createSemaphore(vulkanInstance.logicalDevice, signalSemaphores[i]);
    }
}

void FrameWork::drawGeometry(){

}

void FrameWork::drawFrame(){

//    vkAcquireNextImageKHR(vulkanInstance.logicalDevice,
//                          vulkanSwapChain.swapchain,
//                          UINT64_MAX,
//                          VK_NULL_HANDLE,
//                          VK_NULL_HANDLE,
//                          &imgIndx);
//
//    VkPresentInfoKHR presentInfo {
///* VkStructureType       */ .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
///* const void*           */ .pNext = nullptr,
///* uint32_t              */ .waitSemaphoreCount = 1,
///* const VkSemaphore*    */ .pWaitSemaphores = &imgAvailSemaphores[imgIndx],
///* uint32_t              */ .swapchainCount = 1,
///* const VkSwapchainKHR* */ .pSwapchains = &vulkanSwapChain.swapchain,
///* const uint32_t*       */ .pImageIndices = &imgIndx,
///* VkResult*             */ .pResults = NULL,
//    };
//
//    vkQueuePresentKHR(vulkanInstance.queueInfos[IDX_GRAPHICS].queues[0], &presentInfo);
}

void FrameWork::recordCommands() {

//    vkCmdBindVertexBuffers(gfxCommandPoolInfo.commandBuffers[0],
//                           0,
//                           1,
//                           &vertexBufferInfo.buffer,
//                           &offset);
//
//    vkCmdBindPipeline(gfxCommandPoolInfo.commandBuffers[0],
//                      VK_PIPELINE_BIND_POINT_GRAPHICS,
//                      vulkanRender.gfxPipeline);
//
//    vkCmdDrawIndexed(gfxCommandPoolInfo.commandBuffers[0],
//                     6,
//                     1,
//                     0,
//                     0,
//                     0);
//
//    H_endCommandBufferRecording(gfxCommandPoolInfo.commandBuffers[0]);
}



