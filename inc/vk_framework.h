#ifndef VK_FRAMEWORK_H
#define VK_FRAMEWORK_H

#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		Log::error("Fatal : VkResult is \" %s \" in %s at line %d", string_VkResult(res) , __FILE__, __LINE__); \
		std::abort();																		\
	}																									\
}

class FrameWork : public DrawLooper{
private:
    ContextType contextType;
    VulkanInstance vulkanInstance;
    VulkanSwapChain vulkanSwapChain;
    CommandPoolInfo gfxCommandPoolInfo;
    CommandPoolInfo trxCommandPoolInfo;

    GpuRenderPass gpuRenderPass;

    BufferInfo vertexBufferInfo{};
    BufferInfo indexBufferInfo{};

    void setupSwapChain();
    void setupRenderPass();
    void setupCommandPool();

    void draw() override;

public:

#if defined _WIN32
    explicit FrameWork(ContextType, WindowWindows&);
    WindowWindows window;
#elif defined __linux
    explicit FrameWork(ContextType mcontextType, WindowLinux& mWindowLinux);
    WindowLinux window;
#elif defined __ANDROID__
    explicit FrameWork(ContextType, WindowAndroid&);
    WindowAndroid window;
#endif
    ~FrameWork();

    // Delete this later
    uint32_t imgIndx = 0;
    std::vector<VkFence> inFlightFences;
    std::vector<VkSemaphore> imgAvailSemaphores;
    std::vector<VkSemaphore> signalSemaphores;

    std::vector<VkPipelineStageFlags> wait_stages = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    // Delete this later

    bool InitRenderEngine();
    bool LoadScene(const char* path);
    bool UseRenderType(RenderStyle);
    bool LoadLightStyle(LightData*, const char* lightData);

    void setupSyncObjects();
    void setupGeom();
    void recordCommands();
    void drawGeometry();
    void drawFrame();

    void mainLoop();

    void cleanup();


};

#endif // VK_FRAMEWORK_H