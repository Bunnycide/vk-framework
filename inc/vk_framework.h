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

    VkRenderPassBeginInfo renderPassBeginInfo;

    Texture2D texture;

    DrawSetup drawSetup;
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

    bool InitRenderEngine();
    bool LoadScene(const char* path);
    bool UseRenderType(RenderStyle);
    bool LoadLightStyle(LightData*, const char* lightData);

    void setupSyncObjects();
    void setupGeom();
    void recordCommands();
    void drawFrame();

    void mainLoop();

    void cleanup();
    ;
};

#endif // VK_FRAMEWORK_H